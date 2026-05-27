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
  api_endpoint   = module.http_lambda.api_gateway_url
}

module "http_lambda" {
  source                  = "../modules/http-lambda"
  scenario_name           = var.scenario_name
  lambda_zip_dir          = "${path.root}/../../../lambda"
  dynamo_table_name       = module.data.dynamo_table_name
  dynamo_table_arn        = module.data.dynamo_table_arn
  redis_endpoint          = module.data.redis_endpoint
  matchmaking_config_name = module.gamelift.matchmaking_config_name
  private_subnet_ids      = module.networking.private_subnet_ids
  lambda_sg_id            = module.networking.lambda_sg_id
}
