locals {
  prefix = "gomoku-${var.scenario_name}"
  tags   = { Scenario = var.scenario_name }

  common_env = {
    DYNAMO_TABLE            = var.dynamo_table_name
    MATCHMAKING_CONFIG_NAME = var.matchmaking_config_name
    REDIS                   = var.redis_endpoint
  }
}

# ── Lambda ZIPs ───────────────────────────────────────────────────────────────
data "archive_file" "match_request" {
  type        = "zip"
  source_dir  = "${var.lambda_zip_dir}/GameMatchRequest"
  output_path = "${path.module}/builds/game-match-request.zip"
}

data "archive_file" "match_status" {
  type        = "zip"
  source_dir  = "${var.lambda_zip_dir}/GameMatchStatus"
  output_path = "${path.module}/builds/game-match-status.zip"
}

data "archive_file" "rank_reader" {
  type        = "zip"
  source_dir  = "${var.lambda_zip_dir}/GameRankReader"
  output_path = "${path.module}/builds/game-rank-reader.zip"
}

# ── IAM Role (shared across all 3 HTTP Lambdas) ───────────────────────────────
resource "aws_iam_role" "http_lambda" {
  name = "${local.prefix}-role-http-lambda"
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

resource "aws_iam_role_policy" "http_lambda" {
  name = "policy"
  role = aws_iam_role.http_lambda.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [
      {
        Effect   = "Allow"
        Action   = ["dynamodb:GetItem", "dynamodb:PutItem", "dynamodb:UpdateItem"]
        Resource = var.dynamo_table_arn
      },
      {
        Effect   = "Allow"
        Action   = ["gamelift:StartMatchmaking", "gamelift:DescribeMatchmaking", "gamelift:StopMatchmaking"]
        Resource = "*"
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
resource "aws_lambda_function" "match_request" {
  function_name    = "${local.prefix}-game-match-request"
  filename         = data.archive_file.match_request.output_path
  source_code_hash = data.archive_file.match_request.output_base64sha256
  runtime          = "python3.11"
  handler          = "index.lambda_handler"
  role             = aws_iam_role.http_lambda.arn
  timeout          = 60

  vpc_config {
    subnet_ids         = var.private_subnet_ids
    security_group_ids = [var.lambda_sg_id]
  }

  environment {
    variables = local.common_env
  }

  tags = merge(local.tags, { Name = "${local.prefix}-game-match-request" })
}

resource "aws_lambda_function" "match_status" {
  function_name    = "${local.prefix}-game-match-status"
  filename         = data.archive_file.match_status.output_path
  source_code_hash = data.archive_file.match_status.output_base64sha256
  runtime          = "python3.11"
  handler          = "index.lambda_handler"
  role             = aws_iam_role.http_lambda.arn
  timeout          = 60

  vpc_config {
    subnet_ids         = var.private_subnet_ids
    security_group_ids = [var.lambda_sg_id]
  }

  environment {
    variables = local.common_env
  }

  tags = merge(local.tags, { Name = "${local.prefix}-game-match-status" })
}

resource "aws_lambda_function" "rank_reader" {
  function_name    = "${local.prefix}-game-rank-reader"
  filename         = data.archive_file.rank_reader.output_path
  source_code_hash = data.archive_file.rank_reader.output_base64sha256
  runtime          = "python3.11"
  handler          = "index.handler"
  role             = aws_iam_role.http_lambda.arn
  timeout          = 60

  vpc_config {
    subnet_ids         = var.private_subnet_ids
    security_group_ids = [var.lambda_sg_id]
  }

  environment {
    variables = local.common_env
  }

  tags = merge(local.tags, { Name = "${local.prefix}-game-rank-reader" })
}

# ── CloudWatch Log Groups ─────────────────────────────────────────────────────
resource "aws_cloudwatch_log_group" "match_request" {
  name              = "/aws/lambda/${aws_lambda_function.match_request.function_name}"
  retention_in_days = 7
  tags              = local.tags
}

resource "aws_cloudwatch_log_group" "match_status" {
  name              = "/aws/lambda/${aws_lambda_function.match_status.function_name}"
  retention_in_days = 7
  tags              = local.tags
}

resource "aws_cloudwatch_log_group" "rank_reader" {
  name              = "/aws/lambda/${aws_lambda_function.rank_reader.function_name}"
  retention_in_days = 7
  tags              = local.tags
}

# ── API Gateway REST API ──────────────────────────────────────────────────────
resource "aws_api_gateway_rest_api" "main" {
  name = "${local.prefix}-api"
  endpoint_configuration {
    types = ["REGIONAL"]
  }
  tags = merge(local.tags, { Name = "${local.prefix}-api" })
}

# /match resource
resource "aws_api_gateway_resource" "match" {
  rest_api_id = aws_api_gateway_rest_api.main.id
  parent_id   = aws_api_gateway_rest_api.main.root_resource_id
  path_part   = "match"
}

# POST /match
resource "aws_api_gateway_method" "match_post" {
  rest_api_id   = aws_api_gateway_rest_api.main.id
  resource_id   = aws_api_gateway_resource.match.id
  http_method   = "POST"
  authorization = "NONE"
}

resource "aws_api_gateway_integration" "match_post" {
  rest_api_id             = aws_api_gateway_rest_api.main.id
  resource_id             = aws_api_gateway_resource.match.id
  http_method             = aws_api_gateway_method.match_post.http_method
  integration_http_method = "POST"
  type                    = "AWS_PROXY"
  uri                     = aws_lambda_function.match_request.invoke_arn
}

resource "aws_lambda_permission" "match_request" {
  statement_id  = "AllowAPIGateway"
  action        = "lambda:InvokeFunction"
  function_name = aws_lambda_function.match_request.function_name
  principal     = "apigateway.amazonaws.com"
  source_arn    = "${aws_api_gateway_rest_api.main.execution_arn}/*/*"
}

# GET /match
resource "aws_api_gateway_method" "match_get" {
  rest_api_id   = aws_api_gateway_rest_api.main.id
  resource_id   = aws_api_gateway_resource.match.id
  http_method   = "GET"
  authorization = "NONE"
}

resource "aws_api_gateway_integration" "match_get" {
  rest_api_id             = aws_api_gateway_rest_api.main.id
  resource_id             = aws_api_gateway_resource.match.id
  http_method             = aws_api_gateway_method.match_get.http_method
  integration_http_method = "POST"
  type                    = "AWS_PROXY"
  uri                     = aws_lambda_function.match_status.invoke_arn
}

resource "aws_lambda_permission" "match_status" {
  statement_id  = "AllowAPIGateway"
  action        = "lambda:InvokeFunction"
  function_name = aws_lambda_function.match_status.function_name
  principal     = "apigateway.amazonaws.com"
  source_arn    = "${aws_api_gateway_rest_api.main.execution_arn}/*/*"
}

# /rank resource
resource "aws_api_gateway_resource" "rank" {
  rest_api_id = aws_api_gateway_rest_api.main.id
  parent_id   = aws_api_gateway_rest_api.main.root_resource_id
  path_part   = "rank"
}

# GET /rank
resource "aws_api_gateway_method" "rank_get" {
  rest_api_id   = aws_api_gateway_rest_api.main.id
  resource_id   = aws_api_gateway_resource.rank.id
  http_method   = "GET"
  authorization = "NONE"
}

resource "aws_api_gateway_integration" "rank_get" {
  rest_api_id             = aws_api_gateway_rest_api.main.id
  resource_id             = aws_api_gateway_resource.rank.id
  http_method             = aws_api_gateway_method.rank_get.http_method
  integration_http_method = "POST"
  type                    = "AWS_PROXY"
  uri                     = aws_lambda_function.rank_reader.invoke_arn
}

resource "aws_lambda_permission" "rank_reader" {
  statement_id  = "AllowAPIGateway"
  action        = "lambda:InvokeFunction"
  function_name = aws_lambda_function.rank_reader.function_name
  principal     = "apigateway.amazonaws.com"
  source_arn    = "${aws_api_gateway_rest_api.main.execution_arn}/*/*"
}

# ── Deployment & Stage ────────────────────────────────────────────────────────
resource "aws_api_gateway_deployment" "main" {
  rest_api_id = aws_api_gateway_rest_api.main.id

  depends_on = [
    aws_api_gateway_integration.match_post,
    aws_api_gateway_integration.match_get,
    aws_api_gateway_integration.rank_get,
  ]

  # Force redeployment when integrations change
  triggers = {
    redeployment = sha1(jsonencode([
      aws_api_gateway_integration.match_post,
      aws_api_gateway_integration.match_get,
      aws_api_gateway_integration.rank_get,
    ]))
  }

  lifecycle {
    create_before_destroy = true
  }
}

resource "aws_api_gateway_stage" "v1" {
  deployment_id = aws_api_gateway_deployment.main.id
  rest_api_id   = aws_api_gateway_rest_api.main.id
  stage_name    = "v1"
  tags          = merge(local.tags, { Name = "${local.prefix}-api-stage" })
}
