output "api_endpoint" {
  description = "API Gateway invoke URL for Scenario A (Lambda backend)"
  value       = module.http_lambda.api_gateway_url
}

output "cloudfront_url" {
  description = "CloudFront URL for the web UI"
  value       = module.static_frontend.cloudfront_url
}

output "dynamodb_table_name" {
  value = module.data.dynamo_table_name
}

output "redis_endpoint" {
  value = module.data.redis_endpoint
}

output "gamelift_fleet_id" {
  value = module.gamelift.fleet_id
}

output "match_request_function_name" {
  description = "Lambda function name for cold-start testing"
  value       = module.http_lambda.match_request_function_name
}
