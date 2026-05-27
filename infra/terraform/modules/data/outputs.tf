output "dynamo_table_name" {
  value = aws_dynamodb_table.players.name
}

output "dynamo_table_arn" {
  value = aws_dynamodb_table.players.arn
}

output "dynamo_stream_arn" {
  value = aws_dynamodb_table.players.stream_arn
}

output "redis_endpoint" {
  value = aws_elasticache_cluster.ranking.cache_nodes[0].address
}

output "queue_url" {
  value = aws_sqs_queue.game_results.url
}

output "queue_arn" {
  value = aws_sqs_queue.game_results.arn
}
