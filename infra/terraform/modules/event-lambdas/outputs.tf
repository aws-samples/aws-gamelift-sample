output "sqs_processor_arn" {
  value = aws_lambda_function.sqs_process.arn
}

output "rank_updater_arn" {
  value = aws_lambda_function.rank_update.arn
}

output "match_event_arn" {
  value = aws_lambda_function.match_event.arn
}
