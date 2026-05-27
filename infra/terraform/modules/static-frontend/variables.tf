variable "scenario_name" {
  type        = string
  description = "Short scenario identifier."
}

variable "web_source_dir" {
  type        = string
  description = "Absolute path to the web/ directory to upload."
}

variable "api_endpoint" {
  type        = string
  description = "Base URL of the HTTP API (API Gateway or ALB) injected into main.js."
}

variable "alb_dns_name" {
  type        = string
  description = "ALB DNS name (Scenario B only). When set, CloudFront proxies /match and /rank to ALB and main.js uses the CloudFront HTTPS URL."
  default     = ""
}
