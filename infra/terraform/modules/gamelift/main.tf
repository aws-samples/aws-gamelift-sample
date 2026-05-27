locals {
  prefix           = "gomoku-${var.scenario_name}"
  tags             = { Scenario = var.scenario_name }
  create_fleet     = var.existing_fleet_arn == ""
  effective_fleet_arn = local.create_fleet ? aws_gamelift_fleet.main[0].arn : var.existing_fleet_arn
  effective_fleet_id  = local.create_fleet ? aws_gamelift_fleet.main[0].id : regex("fleet/(.+)$", var.existing_fleet_arn)[0]
}

data "aws_caller_identity" "current" {}

# ── IAM: Fleet Instance Role (EC2 instance profile for GameLift fleet) ───────
resource "aws_iam_role" "fleet_instance" {
  name = "${local.prefix}-fleet-role"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Effect    = "Allow"
        Principal = { Service = "ec2.amazonaws.com" }
        Action    = "sts:AssumeRole"
      },
      {
        Effect    = "Allow"
        Principal = { Service = "gamelift.amazonaws.com" }
        Action    = "sts:AssumeRole"
      }
    ]
  })
  tags = local.tags
}

# Allow fleet instances to assume the SQS sender role
resource "aws_iam_role_policy" "fleet_instance_sts" {
  name = "assume-sqs-sender"
  role = aws_iam_role.fleet_instance.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Effect   = "Allow"
      Action   = "sts:AssumeRole"
      Resource = aws_iam_role.gamelift_sqs_sender.arn
    }]
  })
}

resource "aws_iam_instance_profile" "fleet" {
  name = "${local.prefix}-fleet-profile"
  role = aws_iam_role.fleet_instance.name
  tags = local.tags
}

# ── IAM: SQS Sender Role (assumed by C++ server via ROLE_ARN in config.ini) ──
# Separate role avoids circular self-trust in fleet_instance trust policy.
resource "aws_iam_role" "gamelift_sqs_sender" {
  name = "${local.prefix}-gamelift-sqs-sender"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Effect    = "Allow"
      Principal = { AWS = aws_iam_role.fleet_instance.arn }
      Action    = "sts:AssumeRole"
    }]
  })
  tags = local.tags
}

resource "aws_iam_role_policy" "gamelift_sqs_sender" {
  name = "sqs-send-results"
  role = aws_iam_role.gamelift_sqs_sender.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Effect   = "Allow"
      Action   = ["sqs:SendMessage", "sqs:GetQueueUrl"]
      Resource = var.sqs_queue_arn
    }]
  })
}

# ── IAM Role for GameLift to read build artifacts from S3 ────────────────────
resource "aws_iam_role" "gamelift_s3_access" {
  name = "${local.prefix}-gamelift-s3-access"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Effect    = "Allow"
      Principal = { Service = "gamelift.amazonaws.com" }
      Action    = "sts:AssumeRole"
    }]
  })
  tags = local.tags
}

resource "aws_iam_role_policy" "gamelift_s3_access" {
  name = "s3-build-read"
  role = aws_iam_role.gamelift_s3_access.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Effect   = "Allow"
      Action   = ["s3:GetObject"]
      Resource = "arn:aws:s3:::${var.build_s3_bucket}/${var.build_s3_key}"
    }]
  })
}

# ── GameLift Build ────────────────────────────────────────────────────────────
resource "aws_gamelift_build" "server" {
  name             = "${local.prefix}-GomokuServer"
  operating_system = "WINDOWS_2016"

  storage_location {
    bucket   = var.build_s3_bucket
    key      = var.build_s3_key
    role_arn = aws_iam_role.gamelift_s3_access.arn
  }

  tags = merge(local.tags, { Name = "${local.prefix}-build" })

  lifecycle {
    ignore_changes = [storage_location]
  }
}

# ── GameLift Fleet ────────────────────────────────────────────────────────────
resource "aws_gamelift_fleet" "main" {
  count             = local.create_fleet ? 1 : 0
  name              = "${local.prefix}-fleet"
  build_id          = aws_gamelift_build.server.id
  ec2_instance_type = var.fleet_ec2_instance_type
  instance_role_arn = aws_iam_role.fleet_instance.arn

  # GomokuServer listens on a single TCP port per session
  ec2_inbound_permission {
    from_port = 49152
    to_port   = 60000
    ip_range  = "0.0.0.0/0"
    protocol  = "TCP"
  }

  runtime_configuration {
    server_process {
      concurrent_executions = 1
      launch_path           = "C:\\game\\Binaries\\Win64\\GomokuServer.exe"
      parameters            = " "
    }
  }

  resource_creation_limit_policy {
    new_game_sessions_per_creator = 5
    policy_period_in_minutes      = 15
  }

  tags = merge(local.tags, { Name = "${local.prefix}-fleet" })

  # Fleet activation can take several minutes
  timeouts {
    create = "30m"
    delete = "30m"
  }

  lifecycle {
    ignore_changes = [build_id, instance_role_arn, runtime_configuration, resource_creation_limit_policy, ec2_inbound_permission, description]
  }
}

# ── GameLift Scaling Policy ───────────────────────────────────────────────────
resource "aws_gamelift_game_session_queue" "main" {
  name                  = "${local.prefix}-queue"
  timeout_in_seconds    = 30
  destinations          = [local.effective_fleet_arn]

  tags = merge(local.tags, { Name = "${local.prefix}-queue" })
}

# ── FlexMatch Rule Set + Matchmaking Config (via CloudFormation) ──────────────
# aws_gamelift_matchmaking_rule_set and aws_gamelift_matchmaking_configuration
# are not available in Terraform AWS provider 5.x; use CloudFormation instead.
resource "aws_cloudformation_stack" "matchmaking" {
  name = "${local.prefix}-matchmaking"

  template_body = jsonencode({
    Description = "GameLift FlexMatch resources for ${local.prefix}"
    Resources = {
      RuleSet = {
        Type = "AWS::GameLift::MatchmakingRuleSet"
        Properties = {
          Name        = "${local.prefix}-GomokuRuleSet"
          RuleSetBody = var.ruleset_body
        }
      }
      Config = {
        Type      = "AWS::GameLift::MatchmakingConfiguration"
        DependsOn = ["RuleSet"]
        Properties = {
          Name                  = "${local.prefix}-GomokuMatchConfig"
          RuleSetName           = "${local.prefix}-GomokuRuleSet"
          GameSessionQueueArns  = [aws_gamelift_game_session_queue.main.arn]
          RequestTimeoutSeconds = 30
          AcceptanceRequired    = false
          BackfillMode          = "MANUAL"
        }
      }
    }
  })

  depends_on = [aws_gamelift_game_session_queue.main]
  tags       = local.tags

  lifecycle {
    ignore_changes = [template_body]
  }
}

# ── SNS Topic for GameLift Matchmaking Events ─────────────────────────────────
resource "aws_sns_topic" "matchmaking_events" {
  name = "${local.prefix}-matchmaking-events"
  tags = merge(local.tags, { Name = "${local.prefix}-matchmaking-events" })
}

# ── EventBridge Rule: GameLift matchmaking → SNS ──────────────────────────────
resource "aws_cloudwatch_event_rule" "matchmaking" {
  name        = "${local.prefix}-gamelift-matchmaking"
  description = "Capture GameLift FlexMatch matchmaking events"

  event_pattern = jsonencode({
    source      = ["aws.gamelift"]
    detail-type = ["GameLift Matchmaking Event"]
    detail = {
      type = ["MatchmakingSucceeded", "MatchmakingFailed", "MatchmakingTimedOut", "MatchmakingCancelled"]
    }
  })

  tags = local.tags
}

resource "aws_cloudwatch_event_target" "matchmaking_sns" {
  rule      = aws_cloudwatch_event_rule.matchmaking.name
  target_id = "MatchmakingSNS"
  arn       = aws_sns_topic.matchmaking_events.arn
}

# Allow EventBridge to publish to SNS
resource "aws_sns_topic_policy" "matchmaking_events" {
  arn = aws_sns_topic.matchmaking_events.arn

  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Sid    = "AllowEventBridge"
      Effect = "Allow"
      Principal = {
        Service = "events.amazonaws.com"
      }
      Action   = "SNS:Publish"
      Resource = aws_sns_topic.matchmaking_events.arn
    }]
  })
}
