locals {
  prefix       = "gomoku-${var.scenario_name}"
  tags         = { Scenario = var.scenario_name }
  has_alb      = var.alb_dns_name != ""
  # When ALB is proxied through CloudFront, main.js uses the CF HTTPS URL.
  # Otherwise use the passed api_endpoint directly (Scenario A: API Gateway HTTPS).
  effective_api = local.has_alb ? "https://${aws_cloudfront_distribution.web.domain_name}" : var.api_endpoint
}

# ── S3 Bucket ─────────────────────────────────────────────────────────────────
resource "aws_s3_bucket" "web" {
  bucket        = "${local.prefix}-web-ui"
  force_destroy = true
  tags          = merge(local.tags, { Name = "${local.prefix}-web-ui" })
}

resource "aws_s3_bucket_public_access_block" "web" {
  bucket                  = aws_s3_bucket.web.id
  block_public_acls       = true
  block_public_policy     = true
  ignore_public_acls      = true
  restrict_public_buckets = true
}

# ── Origin Access Control ─────────────────────────────────────────────────────
resource "aws_cloudfront_origin_access_control" "web" {
  name                              = "${local.prefix}-web-oac"
  origin_access_control_origin_type = "s3"
  signing_behavior                  = "always"
  signing_protocol                  = "sigv4"
}

# ── CloudFront Distribution ───────────────────────────────────────────────────
resource "aws_cloudfront_distribution" "web" {
  enabled             = true
  default_root_object = "index.html"
  comment             = "Gomoku web UI - scenario ${var.scenario_name}"

  origin {
    domain_name              = aws_s3_bucket.web.bucket_regional_domain_name
    origin_id                = "s3-web"
    origin_access_control_id = aws_cloudfront_origin_access_control.web.id
  }

  dynamic "origin" {
    for_each = local.has_alb ? [1] : []
    content {
      domain_name = var.alb_dns_name
      origin_id   = "alb-api"
      custom_origin_config {
        http_port              = 80
        https_port             = 443
        origin_protocol_policy = "http-only"
        origin_ssl_protocols   = ["TLSv1.2"]
      }
    }
  }

  # API path behaviors (only when ALB origin present)
  dynamic "ordered_cache_behavior" {
    for_each = local.has_alb ? ["/match", "/match/*", "/rank", "/health"] : []
    content {
      path_pattern           = ordered_cache_behavior.value
      target_origin_id       = "alb-api"
      viewer_protocol_policy = "redirect-to-https"
      allowed_methods        = ["DELETE", "GET", "HEAD", "OPTIONS", "PATCH", "POST", "PUT"]
      cached_methods         = ["GET", "HEAD"]

      forwarded_values {
        query_string = true
        headers      = ["Content-Type", "Accept"]
        cookies {
          forward = "none"
        }
      }

      min_ttl     = 0
      default_ttl = 0
      max_ttl     = 0
    }
  }

  default_cache_behavior {
    target_origin_id       = "s3-web"
    viewer_protocol_policy = "redirect-to-https"
    allowed_methods        = ["GET", "HEAD"]
    cached_methods         = ["GET", "HEAD"]

    forwarded_values {
      query_string = false
      cookies {
        forward = "none"
      }
    }

    min_ttl     = 0
    default_ttl = 3600
    max_ttl     = 86400
  }

  restrictions {
    geo_restriction {
      restriction_type = "none"
    }
  }

  viewer_certificate {
    cloudfront_default_certificate = true
  }

  tags = merge(local.tags, { Name = "${local.prefix}-cloudfront" })
}

# ── S3 Bucket Policy: allow CloudFront OAC ───────────────────────────────────
resource "aws_s3_bucket_policy" "web" {
  bucket = aws_s3_bucket.web.id
  policy = jsonencode({
    Version = "2012-10-17"
    Statement = [{
      Sid    = "AllowCloudFront"
      Effect = "Allow"
      Principal = {
        Service = "cloudfront.amazonaws.com"
      }
      Action   = "s3:GetObject"
      Resource = "${aws_s3_bucket.web.arn}/*"
      Condition = {
        StringEquals = {
          "AWS:SourceArn" = aws_cloudfront_distribution.web.arn
        }
      }
    }]
  })
}

# ── Upload web/ files ─────────────────────────────────────────────────────────
locals {
  web_files = fileset(var.web_source_dir, "**")

  mime_map = {
    ".html" = "text/html"
    ".js"   = "application/javascript"
    ".css"  = "text/css"
    ".png"  = "image/png"
    ".jpg"  = "image/jpeg"
    ".svg"  = "image/svg+xml"
    ".json" = "application/json"
    ".ico"  = "image/x-icon"
  }
}

resource "aws_s3_object" "web_files" {
  for_each = { for f in toset(local.web_files) : f => f if f != "main.js" }

  bucket       = aws_s3_bucket.web.id
  key          = each.value
  source       = "${var.web_source_dir}/${each.value}"
  etag         = filemd5("${var.web_source_dir}/${each.value}")
  content_type = lookup(local.mime_map, regex("\\.[^.]+$", each.value), "application/octet-stream")
  tags         = local.tags
}

resource "aws_s3_object" "main_js" {
  bucket       = aws_s3_bucket.web.id
  key          = "main.js"
  content      = templatefile("${var.web_source_dir}/main.js", { api_endpoint = local.effective_api })
  etag         = md5(templatefile("${var.web_source_dir}/main.js", { api_endpoint = local.effective_api }))
  content_type = "application/javascript"
  tags         = local.tags
}
