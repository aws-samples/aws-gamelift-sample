output "api_gateway_url" {
  value = "${aws_api_gateway_stage.v1.invoke_url}"
}

output "api_gateway_id" {
  value = aws_api_gateway_rest_api.main.id
}

output "match_request_function_name" {
  value = aws_lambda_function.match_request.function_name
}

output "rank_reader_function_name" {
  value = aws_lambda_function.rank_reader.function_name
}
