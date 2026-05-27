$ErrorActionPreference = "Stop"
$FLEET_ID  = "fleet-f386cbc8-f276-40fc-a275-e6f0dc5e1e41"
$CLUSTER   = "gomoku-b-cluster"
$SERVICE   = "gomoku-b-service"
$SCALE_RES = "service/$CLUSTER/$SERVICE"

Write-Host "=== STOPPING ALL RESOURCES ==="

# Step 1: set autoscaling min to 0 so it stops fighting desired=0
Write-Host "[1/4] Setting ECS autoscaling min -> 0 ..."
aws application-autoscaling register-scalable-target `
  --service-namespace ecs `
  --resource-id $SCALE_RES `
  --scalable-dimension ecs:service:DesiredCount `
  --min-capacity 0 --max-capacity 10 | Out-Null
Write-Host "      done."

# Step 2: scale ECS service to 0
Write-Host "[2/4] Scaling ECS service to 0 tasks ..."
aws ecs update-service --cluster $CLUSTER --service $SERVICE `
  --desired-count 0 --query "service.desiredCount" --output text | Out-Null

$waited = 0
$running = "99"
do {
    Start-Sleep -Seconds 5
    $waited += 5
    $running = (aws ecs describe-services --cluster $CLUSTER --services $SERVICE `
      --query "services[0].runningCount" --output text)
    Write-Host "      running tasks: $running (${waited}s)"
} while ($running -ne "0" -and $waited -lt 120)

if ($running -eq "0") {
    Write-Host "      ECS stopped. OK"
} else {
    Write-Host "      WARNING: tasks still running after 120s"
}

# Step 3: suspend GameLift fleet
Write-Host "[3/4] Suspending GameLift fleet (desired 0) ..."
aws gamelift update-fleet-capacity `
  --fleet-id $FLEET_ID `
  --min-size 0 --max-size 1 --desired-instances 0 `
  --query "FleetId" --output text | Out-Null
Write-Host "      request sent (EC2 drains ~2-5 min in background)."

# Step 4: summary
Write-Host ""
Write-Host "[4/4] Summary"
Write-Host "  STOPPED  ECS Fargate tasks        saves ~0.60/day"
Write-Host "  STOPPED  GameLift EC2 instances   saves ~2.40/day"
Write-Host ""
Write-Host "  STILL ON  NAT Gateways x2         ~2.16/day"
Write-Host "  STILL ON  ElastiCache x2           ~0.82/day"
Write-Host "  STILL ON  ALB                      ~0.54/day"
Write-Host "  Floor while stopped:               ~3.52/day"
Write-Host ""
Write-Host "Run .\start_all.ps1 before your next test session."
