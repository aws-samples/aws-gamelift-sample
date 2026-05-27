output "api_endpoint" {
  description = "ALB DNS URL for Scenario B (ECS Fargate backend)"
  value       = "http://${module.http_ecs.alb_dns_name}"
}

output "cloudfront_url" {
  description = "CloudFront URL for the web UI"
  value       = module.static_frontend.cloudfront_url
}

output "ecr_repository_url" {
  description = "ECR repository URL — needed for docker push"
  value       = module.http_ecs.ecr_repository_url
}

output "ecs_cluster_name" {
  value = module.http_ecs.ecs_cluster_name
}

output "ecs_service_name" {
  value = module.http_ecs.ecs_service_name
}

output "dynamodb_table_name" {
  value = module.data.dynamo_table_name
}

output "gamelift_fleet_id" {
  value = module.gamelift.fleet_id
}
