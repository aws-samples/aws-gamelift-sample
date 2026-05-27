variable "scenario_name" {
  type        = string
  description = "Short scenario identifier."
}

variable "lambda_zip_dir" {
  type        = string
  description = "Absolute path to the lambda/ directory."
}

variable "dynamo_table_name" {
  type        = string
  description = "DynamoDB table name."
}

variable "dynamo_table_arn" {
  type        = string
  description = "DynamoDB table ARN."
}

variable "redis_endpoint" {
  type        = string
  description = "ElastiCache Redis endpoint hostname."
}

variable "matchmaking_config_name" {
  type        = string
  description = "GameLift matchmaking configuration name."
}

variable "private_subnet_ids" {
  type        = list(string)
  description = "Private subnet IDs for Lambda VPC config."
}

variable "lambda_sg_id" {
  type        = string
  description = "Security group ID for Lambda functions."
}
