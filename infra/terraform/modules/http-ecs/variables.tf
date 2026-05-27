variable "scenario_name" {
  type        = string
  description = "Short scenario identifier."
}

variable "region" {
  type        = string
  description = "AWS region."
}

variable "image_tag" {
  type        = string
  description = "Docker image tag to deploy (ECR image tag)."
  default     = "latest"
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

variable "vpc_id" {
  type        = string
  description = "VPC ID."
}

variable "public_subnet_ids" {
  type        = list(string)
  description = "Public subnet IDs for the ALB."
}

variable "private_subnet_ids" {
  type        = list(string)
  description = "Private subnet IDs for ECS tasks."
}

variable "ecs_sg_id" {
  type        = string
  description = "Security group ID for ECS tasks."
}

variable "alb_sg_id" {
  type        = string
  description = "Security group ID for the ALB."
}
