module "networking" {
  source        = "../modules/networking"
  scenario_name = var.scenario_name
  cidr_block    = var.cidr_block
}

module "data" {
  source             = "../modules/data"
  scenario_name      = var.scenario_name
  private_subnet_ids = module.networking.private_subnet_ids
  elasticache_sg_id  = module.networking.elasticache_sg_id
}

module "gamelift" {
  source                  = "../modules/gamelift"
  scenario_name           = var.scenario_name
  build_s3_bucket         = var.gamelift_build_s3_bucket
  build_s3_key            = var.gamelift_build_s3_key
  fleet_ec2_instance_type = var.fleet_ec2_instance_type
  ruleset_body            = file("${path.root}/../../../ruleset/GomokuRuleSet.json")
  sqs_queue_arn           = module.data.queue_arn
  existing_fleet_arn      = var.existing_fleet_arn
}

module "event_lambdas" {
  source                    = "../modules/event-lambdas"
  scenario_name             = var.scenario_name
  sqs_queue_arn             = module.data.queue_arn
  dynamo_table_name         = module.data.dynamo_table_name
  dynamo_table_arn          = module.data.dynamo_table_arn
  dynamo_stream_arn         = module.data.dynamo_stream_arn
  redis_endpoint            = module.data.redis_endpoint
  private_subnet_ids        = module.networking.private_subnet_ids
  lambda_sg_id              = module.networking.lambda_sg_id
  matchmaking_sns_topic_arn = module.gamelift.matchmaking_sns_topic_arn
  lambda_zip_dir            = "${path.root}/../../../lambda"
}

module "static_frontend" {
  source         = "../modules/static-frontend"
  scenario_name  = var.scenario_name
  web_source_dir = "${path.root}/../../../aws-gamelift-sample/web"
  api_endpoint   = "http://${module.http_ecs.alb_dns_name}"
  alb_dns_name   = module.http_ecs.alb_dns_name
}

module "http_ecs" {
  source                  = "../modules/http-ecs"
  scenario_name           = var.scenario_name
  region                  = var.region
  image_tag               = var.image_tag
  dynamo_table_name       = module.data.dynamo_table_name
  dynamo_table_arn        = module.data.dynamo_table_arn
  redis_endpoint          = module.data.redis_endpoint
  matchmaking_config_name = module.gamelift.matchmaking_config_name
  vpc_id                  = module.networking.vpc_id
  public_subnet_ids       = module.networking.public_subnet_ids
  private_subnet_ids      = module.networking.private_subnet_ids
  ecs_sg_id               = module.networking.ecs_sg_id
  alb_sg_id               = module.networking.alb_sg_id
}
