#!/usr/bin/env bash
# Build the Gomoku backend Docker image and push it to ECR (Scenario B).
#
# Prerequisites:
#   - AWS CLI configured with credentials
#   - Scenario B already deployed (ECR repo must exist)
#   - Docker running locally
#
# Usage (from project root):
#   chmod +x scripts/build_push_ecr.sh
#   ./scripts/build_push_ecr.sh
#
# Override the image tag:
#   IMAGE_TAG=v1.0.0 ./scripts/build_push_ecr.sh

set -euo pipefail

REGION="${AWS_REGION:-us-east-1}"
IMAGE_TAG="${IMAGE_TAG:-latest}"

echo "==> Getting ECR repository URL from Terraform output..."
ECR_URL=$(terraform -chdir=infra/terraform/scenario-b output -raw ecr_repository_url)

ACCOUNT_ID=$(echo "$ECR_URL" | cut -d. -f1)

echo "==> Logging in to ECR..."
aws ecr get-login-password --region "$REGION" \
  | docker login --username AWS --password-stdin "${ACCOUNT_ID}.dkr.ecr.${REGION}.amazonaws.com"

echo "==> Building Docker image..."
docker build \
  -f app/Dockerfile \
  -t "gomoku-backend:${IMAGE_TAG}" \
  .

echo "==> Tagging image..."
docker tag "gomoku-backend:${IMAGE_TAG}" "${ECR_URL}:${IMAGE_TAG}"

echo "==> Pushing image..."
docker push "${ECR_URL}:${IMAGE_TAG}"

echo ""
echo "Done. Image: ${ECR_URL}:${IMAGE_TAG}"
echo ""
echo "Update scenario-b/terraform.tfvars:"
echo "  image_tag = \"${IMAGE_TAG}\""
echo "Then run: terraform -chdir=infra/terraform/scenario-b apply"
