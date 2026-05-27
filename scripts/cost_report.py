"""
Pull Cost Explorer data for both scenarios and print a comparison table.

Usage:
    python scripts/cost_report.py --start 2025-06-01 --end 2025-06-30

Requires:
    - AWS credentials with ce:GetCostAndUsage permission
    - Resources tagged Scenario=a and Scenario=b
    - Cost allocation tags enabled in Billing console for the 'Scenario' tag
"""

import argparse
from datetime import date

import boto3


def get_cost_by_scenario(ce, start: str, end: str, scenario: str) -> dict:
    response = ce.get_cost_and_usage(
        TimePeriod={"Start": start, "End": end},
        Granularity="DAILY",
        Filter={
            "Tags": {
                "Key": "Scenario",
                "Values": [scenario],
                "MatchOptions": ["EQUALS"],
            }
        },
        GroupBy=[{"Type": "DIMENSION", "Key": "SERVICE"}],
        Metrics=["UnblendedCost"],
    )

    totals: dict = {}
    for result in response["ResultsByTime"]:
        for group in result["Groups"]:
            service = group["Keys"][0]
            amount = float(group["Metrics"]["UnblendedCost"]["Amount"])
            totals[service] = totals.get(service, 0.0) + amount

    return totals


def main():
    parser = argparse.ArgumentParser()
    parser.add_argument("--start", default=str(date.today().replace(day=1)))
    parser.add_argument("--end",   default=str(date.today()))
    parser.add_argument("--region", default="us-east-1")
    args = parser.parse_args()

    ce = boto3.client("ce", region_name="us-east-1")  # Cost Explorer is always us-east-1

    costs_a = get_cost_by_scenario(ce, args.start, args.end, "a")
    costs_b = get_cost_by_scenario(ce, args.start, args.end, "b")

    all_services = sorted(set(list(costs_a.keys()) + list(costs_b.keys())))

    print(f"\nCost comparison: {args.start} → {args.end}")
    print(f"{'Service':<45} {'Scenario A ($)':>16} {'Scenario B ($)':>16} {'Diff ($)':>12}")
    print("-" * 93)

    total_a = 0.0
    total_b = 0.0
    for svc in all_services:
        a = costs_a.get(svc, 0.0)
        b = costs_b.get(svc, 0.0)
        total_a += a
        total_b += b
        print(f"{svc:<45} {a:>16.4f} {b:>16.4f} {b - a:>+12.4f}")

    print("-" * 93)
    print(f"{'TOTAL':<45} {total_a:>16.4f} {total_b:>16.4f} {total_b - total_a:>+12.4f}")
    print()

    # Write CSV
    import csv, os
    os.makedirs("results", exist_ok=True)
    with open("results/cost_comparison.csv", "w", newline="") as f:
        writer = csv.writer(f)
        writer.writerow(["service", "scenario_a_usd", "scenario_b_usd", "diff_usd"])
        for svc in all_services:
            a = costs_a.get(svc, 0.0)
            b = costs_b.get(svc, 0.0)
            writer.writerow([svc, f"{a:.4f}", f"{b:.4f}", f"{b - a:+.4f}"])
        writer.writerow(["TOTAL", f"{total_a:.4f}", f"{total_b:.4f}", f"{total_b - total_a:+.4f}"])

    print("Wrote results/cost_comparison.csv")


if __name__ == "__main__":
    main()
