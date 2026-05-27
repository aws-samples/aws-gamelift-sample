terraform {
  backend "s3" {
    bucket         = "gomoku-tfstate-288761753384"
    key            = "scenario-a/terraform.tfstate"
    region         = "us-east-1"
    use_lockfile   = true
    encrypt        = true
  }
}
