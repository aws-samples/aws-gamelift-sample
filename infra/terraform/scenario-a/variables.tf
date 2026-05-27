variable "region" {
  type    = string
  default = "us-east-1"
}

variable "scenario_name" {
  type    = string
  default = "a"
}

variable "cidr_block" {
  type    = string
  default = "10.0.0.0/16"
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

