variable "scenario_name" {
  type        = string
  description = "Short scenario identifier."
}

variable "sqs_queue_arn" {
  type        = string
  description = "ARN of the SQS game results queue."
}

variable "dynamo_table_name" {
  type        = string
  description = "DynamoDB table name."
}

variable "dynamo_table_arn" {
  type        = string
  description = "DynamoDB table ARN."
}

variable "dynamo_stream_arn" {
  type        = string
  description = "DynamoDB stream ARN for rank updates."
}

variable "redis_endpoint" {
  type        = string
  description = "ElastiCache Redis endpoint hostname."
}

variable "private_subnet_ids" {
  type        = list(string)
  description = "Private subnet IDs for Lambda VPC config."
}

variable "lambda_sg_id" {
  type        = string
  description = "Security group ID for Lambda functions."
}

variable "matchmaking_sns_topic_arn" {
  type        = string
  description = "SNS topic ARN for GameLift matchmaking events."
}

variable "lambda_zip_dir" {
  type        = string
  description = "Absolute path to the lambda/ directory containing function subdirectories."
}
