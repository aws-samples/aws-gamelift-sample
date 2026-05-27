output "fleet_id" {
  value = local.effective_fleet_id
}

output "fleet_arn" {
  value = local.effective_fleet_arn
}

output "queue_arn" {
  value = aws_gamelift_game_session_queue.main.arn
}

output "matchmaking_config_name" {
  value      = "${local.prefix}-GomokuMatchConfig"
  depends_on = [aws_cloudformation_stack.matchmaking]
}

output "matchmaking_config_arn" {
  value      = ""
  depends_on = [aws_cloudformation_stack.matchmaking]
}

output "matchmaking_sns_topic_arn" {
  value = aws_sns_topic.matchmaking_events.arn
}
