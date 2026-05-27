variable "region" {
  type    = string
  default = "us-east-1"
}

variable "scenario_name" {
  type    = string
  default = "b"
}

variable "cidr_block" {
  type    = string
  default = "10.1.0.0/16"
}

variable "fleet_ec2_instance_type" {
  type    = string
  default = "c5.large"
}

variable "gamelift_build_s3_bucket" {
  type        = string
  description = "S3 bucket containing the GomokuServer build ZIP."
}

variable "gamelift_build_s3_key" {
  type        = string
  description = "S3 key of the GomokuServer build ZIP."
}

variable "image_tag" {
  type        = string
  description = "ECR image tag for the ECS backend container."
  default     = "latest"
}

variable "existing_fleet_arn" {
  type        = string
  description = "ARN of existing GameLift fleet to reuse (account limit workaround)."
  default     = ""
}
