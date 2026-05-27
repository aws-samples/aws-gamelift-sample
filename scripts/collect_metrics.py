"""
Export CloudWatch metrics for a scenario to CSV.

Usage:
    python scripts/collect_metrics.py --scenario a --region us-east-1
    python scripts/collect_metrics.py --scenario b --region us-east-1

Writes:
    results/scenario_<X>_cloudwatch.csv
"""

import argparse
import csv
import os
from datetime import datetime, timedelta, timezone

import boto3

METRICS = [
    # (Namespace, MetricName, Dimensions, StatType)
    ("AWS/Lambda", "Duration",       "FunctionName", "p95"),
    ("AWS/Lambda", "Duration",       "FunctionName", "p50"),
    ("AWS/Lambda", "Invocations",    "FunctionName", "Sum"),
    ("AWS/Lambda", "Errors",         "FunctionName", "Sum"),
    ("AWS/Lambda", "InitDuration",   "FunctionName", "p95"),
    ("AWS/ApiGateway", "Latency",    "ApiName",      "p95"),
    ("AWS/ApiGateway", "Count",      "ApiName",      "Sum"),
    ("AWS/ECS/ContainerInsights", "RunningTaskCount", "ServiceName", "Average"),
    ("AWS/ApplicationELB", "TargetResponseTime", "LoadBalancer", "p95"),
    ("AWS/ApplicationELB", "RequestCount",       "LoadBalancer", "Sum"),
]


def get_metrics(cw, namespace: str, metric_name: str, dimensions: list, stat: str,
                start: datetime, end: datetime) -> list:
    extended = stat in ("p50", "p90", "p95", "p99")
    kwargs = dict(
        Namespace=namespace,
        MetricName=metric_name,
        Dimensions=dimensions,
        StartTime=start,
        EndTime=end,
        Period=60,
    )
    if extended:
        kwargs["ExtendedStatistics"] = [stat]
    else:
        kwargs["Statistics"] = [stat]

    response = cw.get_metric_statistics(**kwargs)
    points = sorted(response["Datapoints"], key=lambda d: d["Timestamp"])
    if extended:
        return [(p["Timestamp"].isoformat(), p["ExtendedStatistics"].get(stat, 0)) for p in points]
    return [(p["Timestamp"].isoformat(), p.get(stat, 0)) for p in points]


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--scenario", required=True, choices=["a", "b"])
    parser.add_argument("--region", default="us-east-1")
    parser.add_argument("--hours", type=int, default=2, help="Hours of data to pull")
    args = parser.parse_args()

    prefix = f"gomoku-{args.scenario}"
    cw = boto3.client("cloudwatch", region_name=args.region)

    end = datetime.now(timezone.utc)
    start = end - timedelta(hours=args.hours)

    os.makedirs("results", exist_ok=True)
    outfile = f"results/scenario_{args.scenario}_cloudwatch.csv"

    rows = []

    # Lambda metrics
    lambda_client = boto3.client("lambda", region_name=args.region)
    functions = lambda_client.list_functions()["Functions"]
    scenario_fns = [f["FunctionName"] for f in functions if f["FunctionName"].startswith(prefix)]

    for fn_name in scenario_fns:
        for stat in ("p50", "p95", "Invocations", "Errors"):
            metric = "InitDuration" if stat == "p95" else ("Duration" if stat in ("p50", "p95") else stat)
            data = get_metrics(
                cw, "AWS/Lambda", metric,
                [{"Name": "FunctionName", "Value": fn_name}],
                stat, start, end,
            )
            for ts, val in data:
                rows.append({"scenario": args.scenario, "source": fn_name, "metric": f"{metric}_{stat}", "timestamp": ts, "value": val})

    # API Gateway metrics (Scenario A)
    if args.scenario == "a":
        data = get_metrics(
            cw, "AWS/ApiGateway", "Latency",
            [{"Name": "ApiName", "Value": f"{prefix}-api"}],
            "p95", start, end,
        )
        for ts, val in data:
            rows.append({"scenario": args.scenario, "source": f"{prefix}-api", "metric": "Latency_p95", "timestamp": ts, "value": val})

    # ALB metrics (Scenario B)
    if args.scenario == "b":
        elbv2 = boto3.client("elbv2", region_name=args.region)
        lbs = elbv2.describe_load_balancers()["LoadBalancers"]
        scenario_lbs = [lb for lb in lbs if lb["LoadBalancerName"].startswith(prefix)]
        for lb in scenario_lbs:
            lb_suffix = lb["LoadBalancerArn"].split("loadbalancer/")[-1]
            data = get_metrics(
                cw, "AWS/ApplicationELB", "TargetResponseTime",
                [{"Name": "LoadBalancer", "Value": lb_suffix}],
                "p95", start, end,
            )
            for ts, val in data:
                rows.append({"scenario": args.scenario, "source": lb["LoadBalancerName"], "metric": "TargetResponseTime_p95", "timestamp": ts, "value": val})

    with open(outfile, "w", newline="") as f:
        writer = csv.DictWriter(f, fieldnames=["scenario", "source", "metric", "timestamp", "value"])
        writer.writeheader()
        writer.writerows(rows)

    print(f"Wrote {len(rows)} rows to {outfile}")


if __name__ == "__main__":
    main()
