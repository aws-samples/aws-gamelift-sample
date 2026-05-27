$ErrorActionPreference = "Stop"
$FLEET_ID  = "fleet-f386cbc8-f276-40fc-a275-e6f0dc5e1e41"
$CLUSTER   = "gomoku-b-cluster"
$SERVICE   = "gomoku-b-service"
$SCALE_RES = "service/$CLUSTER/$SERVICE"
$ALB_DNS   = "gomoku-b-alb-884112568.us-east-1.elb.amazonaws.com"

Write-Host "=== STARTING ALL RESOURCES ==="

# Step 1: restore ECS autoscaling min to 1
Write-Host "[1/4] Restoring ECS autoscaling min to 1 ..."
aws application-autoscaling register-scalable-target --service-namespace ecs --resource-id $SCALE_RES --scalable-dimension ecs:service:DesiredCount --min-capacity 1 --max-capacity 10 | Out-Null
Write-Host "      done."

# Step 2: scale ECS service to 1
Write-Host "[2/4] Scaling ECS service to 1 task ..."
aws ecs update-service --cluster $CLUSTER --service $SERVICE --desired-count 1 --query "service.desiredCount" --output text | Out-Null

$waited  = 0
$running = "0"
$healthy = $false

do {
    Start-Sleep -Seconds 10
    $waited += 10

    $running = aws ecs describe-services --cluster $CLUSTER --services $SERVICE --query "services[0].runningCount" --output text 2>$null

    if ($running -eq "1") {
        try {
            $resp = Invoke-WebRequest -Uri "http://$ALB_DNS/health" -UseBasicParsing -TimeoutSec 5
            $healthy = ($resp.StatusCode -eq 200)
        } catch {
            $healthy = $false
        }
    }

    if ($healthy) {
        $healthStr = "200 OK"
    } else {
        $healthStr = "not ready"
    }
    Write-Host "      running: $running  ALB /health: $healthStr  (${waited}s)"

} while ((-not $healthy) -and ($waited -lt 300))

if ($healthy) {
    Write-Host "      ECS task running and ALB healthy."
} else {
    Write-Host "      WARNING: ECS/ALB not ready after 300s - check console"
}

# Step 3: restore GameLift fleet
Write-Host "[3/4] Restoring GameLift fleet ..."
aws gamelift update-fleet-capacity --fleet-id $FLEET_ID --min-size 1 --max-size 1 --desired-instances 1 --query "FleetId" --output text | Out-Null
Write-Host "      request sent."

# Step 4: summary
Write-Host ""
Write-Host "[4/4] Ready"
Write-Host "  ECS Fargate : task running, ALB healthy"
Write-Host "  GameLift    : EC2 instance provisioning (~10-15 min)"
Write-Host ""
Write-Host "  Scenario A API : https://x8knjlnw63.execute-api.us-east-1.amazonaws.com/v1"
Write-Host "  Scenario B ALB : http://gomoku-b-alb-884112568.us-east-1.elb.amazonaws.com"
