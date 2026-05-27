variable "scenario_name" {
  type        = string
  description = "Short scenario identifier."
}

variable "private_subnet_ids" {
  type        = list(string)
  description = "Private subnet IDs for ElastiCache subnet group."
}

variable "elasticache_sg_id" {
  type        = string
  description = "Security group ID for ElastiCache cluster."
}
