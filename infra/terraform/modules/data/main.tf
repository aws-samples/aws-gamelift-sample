locals {
  prefix = "gomoku-${var.scenario_name}"
  tags   = { Scenario = var.scenario_name }
}

# ── DynamoDB ──────────────────────────────────────────────────────────────────
resource "aws_dynamodb_table" "players" {
  name         = "${local.prefix}-GomokuPlayerInfo"
  billing_mode = "PAY_PER_REQUEST"
  hash_key     = "PlayerName"

  attribute {
    name = "PlayerName"
    type = "S"
  }

  stream_enabled   = true
  stream_view_type = "NEW_AND_OLD_IMAGES"

  tags = merge(local.tags, { Name = "${local.prefix}-GomokuPlayerInfo" })
}

# ── ElastiCache ───────────────────────────────────────────────────────────────
resource "aws_elasticache_subnet_group" "main" {
  name       = "${local.prefix}-cache-subnet"
  subnet_ids = var.private_subnet_ids
  tags       = local.tags
}

resource "aws_elasticache_cluster" "ranking" {
  cluster_id           = "${local.prefix}-ranking"
  engine               = "redis"
  engine_version       = "7.0"
  node_type            = "cache.t3.micro"
  num_cache_nodes      = 1
  parameter_group_name = "default.redis7"
  port                 = 6379
  subnet_group_name    = aws_elasticache_subnet_group.main.name
  security_group_ids   = [var.elasticache_sg_id]
  tags                 = merge(local.tags, { Name = "${local.prefix}-ranking" })
}

# ── SQS ───────────────────────────────────────────────────────────────────────
resource "aws_sqs_queue" "game_results" {
  name                       = "${local.prefix}-game-result-queue"
  visibility_timeout_seconds = 60

  tags = merge(local.tags, { Name = "${local.prefix}-game-result-queue" })
}
