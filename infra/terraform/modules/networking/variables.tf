variable "scenario_name" {
  type        = string
  description = "Short scenario identifier, e.g. 'a' or 'b'. Used in resource names and tags."
}

variable "cidr_block" {
  type        = string
  description = "VPC CIDR block."
}

