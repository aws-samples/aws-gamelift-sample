locals {
  prefix = "gomoku-${var.scenario_name}"
  tags   = { Scenario = var.scenario_name }

  common_env = {
    DYNAMO_TABLE = var.dynamo_table_name
  }
}

# ── Lambda ZIPs (built at plan time) ─────────────────────────────────────────
data "archive_file" "sqs_process" {
  type        = "zip"
  source_dir  = "${var.lambda_zip_dir}/GameSqsProcess"
  output_path = "${path.module}/builds/game-sqs-process.zip"
}

data "archive_file" "rank_update" {
  type        = "zip"
  source_dir  = "${var.lambda_zip_dir}/GameRankUpdate"
  output_path = "${path.module}/builds/game-rank-update.zip"
}

data "archive_file" "match_event" {
  type        = "zip"
  source_dir  = "${var.lambda_zip_dir}/MatchEvent"
  output_path = "${path.module}/builds/match-event.zip"
}

# ── IAM: SQS Processor ───────────────────────────────────────────────────────
resource "aws_iam_role" "sqs_process" {
  name = "${local.prefix}-role-sqs-process"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Action    = "sts:AssumeRole"
      Effect    = "Allow"
      Principal = { Service = "lambda.amazonaws.com" }
    }]
  })
  tags = local.tags
}

resource "aws_iam_role_policy" "sqs_process" {
  name = "policy"
  role = aws_iam_role.sqs_process.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Effect   = "Allow"
        Action   = ["sqs:ReceiveMessage", "sqs:DeleteMessage", "sqs:GetQueueAttributes", "sqs:ChangeMessageVisibility"]
        Resource = var.sqs_queue_arn
      },
      {
        Effect   = "Allow"
        Action   = ["dynamodb:UpdateItem", "dynamodb:GetItem"]
        Resource = var.dynamo_table_arn
      },
      {
        Effect   = "Allow"
        Action   = ["logs:CreateLogGroup", "logs:CreateLogStream", "logs:PutLogEvents"]
        Resource = "arn:aws:logs:*:*:*"
      },
      {
        Effect   = "Allow"
        Action   = ["ec2:CreateNetworkInterface", "ec2:DescribeNetworkInterfaces", "ec2:DeleteNetworkInterface"]
        Resource = "*"
      }
    ]
  })
}

# ── IAM: Rank Updater ────────────────────────────────────────────────────────
resource "aws_iam_role" "rank_update" {
  name = "${local.prefix}-role-rank-update"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Action    = "sts:AssumeRole"
      Effect    = "Allow"
      Principal = { Service = "lambda.amazonaws.com" }
    }]
  })
  tags = local.tags
}

resource "aws_iam_role_policy" "rank_update" {
  name = "policy"
  role = aws_iam_role.rank_update.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Effect   = "Allow"
        Action   = ["dynamodb:GetRecords", "dynamodb:GetShardIterator", "dynamodb:DescribeStream", "dynamodb:ListStreams"]
        Resource = var.dynamo_stream_arn
      },
      {
        Effect   = "Allow"
        Action   = ["logs:CreateLogGroup", "logs:CreateLogStream", "logs:PutLogEvents"]
        Resource = "arn:aws:logs:*:*:*"
      },
      {
        Effect   = "Allow"
        Action   = ["ec2:CreateNetworkInterface", "ec2:DescribeNetworkInterfaces", "ec2:DeleteNetworkInterface"]
        Resource = "*"
      }
    ]
  })
}

# ── IAM: Match Event ─────────────────────────────────────────────────────────
resource "aws_iam_role" "match_event" {
  name = "${local.prefix}-role-match-event"
  assume_role_policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Action    = "sts:AssumeRole"
      Effect    = "Allow"
      Principal = { Service = "lambda.amazonaws.com" }
    }]
  })
  tags = local.tags
}

resource "aws_iam_role_policy" "match_event" {
  name = "policy"
  role = aws_iam_role.match_event.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Effect   = "Allow"
        Action   = ["dynamodb:UpdateItem", "dynamodb:GetItem"]
        Resource = var.dynamo_table_arn
      },
      {
        Effect   = "Allow"
        Action   = ["logs:CreateLogGroup", "logs:CreateLogStream", "logs:PutLogEvents"]
        Resource = "arn:aws:logs:*:*:*"
      },
      {
        Effect   = "Allow"
        Action   = ["ec2:CreateNetworkInterface", "ec2:DescribeNetworkInterfaces", "ec2:DeleteNetworkInterface"]
        Resource = "*"
      }
    ]
  })
}

# ── Lambda Functions ──────────────────────────────────────────────────────────
resource "aws_lambda_function" "sqs_process" {
  function_name    = "${local.prefix}-game-sqs-process"
  filename         = data.archive_file.sqs_process.output_path
  source_code_hash = data.archive_file.sqs_process.output_base64sha256
  runtime          = "python3.11"
  handler          = "index.lambda_handler"
  role             = aws_iam_role.sqs_process.arn
  timeout          = 60

  vpc_config {
    subnet_ids         = var.private_subnet_ids
    security_group_ids = [var.lambda_sg_id]
  }

  environment {
    variables = local.common_env
  }

  tags = merge(local.tags, { Name = "${local.prefix}-game-sqs-process" })
}

resource "aws_lambda_function" "rank_update" {
  function_name    = "${local.prefix}-game-rank-update"
  filename         = data.archive_file.rank_update.output_path
  source_code_hash = data.archive_file.rank_update.output_base64sha256
  runtime          = "python3.11"
  handler          = "index.handler"
  role             = aws_iam_role.rank_update.arn
  timeout          = 60

  vpc_config {
    subnet_ids         = var.private_subnet_ids
    security_group_ids = [var.lambda_sg_id]
  }

  environment {
    variables = merge(local.common_env, { REDIS = var.redis_endpoint })
  }

  tags = merge(local.tags, { Name = "${local.prefix}-game-rank-update" })
}

resource "aws_lambda_function" "match_event" {
  function_name    = "${local.prefix}-match-event"
  filename         = data.archive_file.match_event.output_path
  source_code_hash = data.archive_file.match_event.output_base64sha256
  runtime          = "python3.11"
  handler          = "index.lambda_handler"
  role             = aws_iam_role.match_event.arn
  timeout          = 60

  vpc_config {
    subnet_ids         = var.private_subnet_ids
    security_group_ids = [var.lambda_sg_id]
  }

  environment {
    variables = local.common_env
  }

  tags = merge(local.tags, { Name = "${local.prefix}-match-event" })
}

# ── Event Source Mappings ─────────────────────────────────────────────────────
resource "aws_lambda_event_source_mapping" "sqs" {
  event_source_arn = var.sqs_queue_arn
  function_name    = aws_lambda_function.sqs_process.arn
  batch_size       = 10
}

resource "aws_lambda_event_source_mapping" "dynamo_stream" {
  event_source_arn  = var.dynamo_stream_arn
  function_name     = aws_lambda_function.rank_update.arn
  starting_position = "LATEST"
  batch_size        = 100
}

# ── SNS Subscription: MatchEvent Lambda ───────────────────────────────────────
resource "aws_sns_topic_subscription" "match_event" {
  topic_arn = var.matchmaking_sns_topic_arn
  protocol  = "lambda"
  endpoint  = aws_lambda_function.match_event.arn
}

resource "aws_lambda_permission" "match_event_sns" {
  statement_id  = "AllowSNSInvoke"
  action        = "lambda:InvokeFunction"
  function_name = aws_lambda_function.match_event.function_name
  principal     = "sns.amazonaws.com"
  source_arn    = var.matchmaking_sns_topic_arn
}

# ── CloudWatch Log Groups ─────────────────────────────────────────────────────
resource "aws_cloudwatch_log_group" "sqs_process" {
  name              = "/aws/lambda/${aws_lambda_function.sqs_process.function_name}"
  retention_in_days = 7
  tags              = local.tags
}

resource "aws_cloudwatch_log_group" "rank_update" {
  name              = "/aws/lambda/${aws_lambda_function.rank_update.function_name}"
  retention_in_days = 7
  tags              = local.tags
}

resource "aws_cloudwatch_log_group" "match_event" {
  name              = "/aws/lambda/${aws_lambda_function.match_event.function_name}"
  retention_in_days = 7
  tags              = local.tags
}
