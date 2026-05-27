variable "scenario_name" {
  type        = string
  description = "Short scenario identifier."
}

variable "build_s3_bucket" {
  type        = string
  description = "S3 bucket containing the GomokuServer build ZIP."
}

variable "build_s3_key" {
  type        = string
  description = "S3 key of the GomokuServer build ZIP."
}

variable "sqs_queue_arn" {
  type        = string
  description = "SQS queue ARN the fleet instances send game results to."
}

variable "fleet_ec2_instance_type" {
  type        = string
  description = "EC2 instance type for the GameLift fleet."
  default     = "c5.large"
}

variable "ruleset_body" {
  type        = string
  description = "JSON body of the FlexMatch matchmaking rule set."
}

variable "existing_fleet_arn" {
  type        = string
  description = "ARN of an existing fleet to reuse. When set, skips fleet creation (use when account fleet limit is reached)."
  default     = ""
}

