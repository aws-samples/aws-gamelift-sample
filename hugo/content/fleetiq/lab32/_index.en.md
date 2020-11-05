---
title: Setting Fiesta IAM
url: /fleetiq/lab32
weight: 50
pre: "<b>3-2. </b>"
---

### Setting IAM for using Fiesta

To use independent FleetIQ, it is needed to make restricted permissions for EC2 instances and Auto Scaling.

1. Login to IAM console. (https://console.aws.amazon.com/iam)

2. Click "Create Policy" on IAM policy menu.

3. Create policy and put JSON schema for it.

![Fiesta](../../images/fleetiq/lab32/Fiesta-1[en].png)

Policy document is configured like below. You can paste this code.

```json
{
    "Version": "2012-10-17",
    "Statement": [
        {
            "Effect": "Allow",
            "Action": "ec2:TerminateInstances",
            "Resource": "*",
            "Condition": {
                "StringEquals": {
                    "ec2:ResourceTag/GameLift": "GameServerGroups"
                }
            }
        },
        {
            "Effect": "Allow",
            "Action": [
                "autoscaling:CompleteLifecycleAction",
                "autoscaling:ResumeProcesses",
                "autoscaling:EnterStandby",
                "autoscaling:SetInstanceProtection",
                "autoscaling:UpdateAutoScalingGroup",
                "autoscaling:SuspendProcesses",
                "autoscaling:DetachInstances"
            ],
            "Resource": "*",
            "Condition": {
                "StringEquals": {
                    "aws:ResourceTag/GameLift": "GameServerGroups"
                }
            }
        },
        {
            "Effect": "Allow",
            "Action": [
                "ec2:DescribeImages",
                "ec2:DescribeInstances",
                "autoscaling:DescribeAutoScalingGroups",
                "ec2:DescribeLaunchTemplateVersions"
            ],
            "Resource": "*"
        },
        {
            "Effect": "Allow",
            "Action": "sns:Publish",
            "Resource": [
                "arn:aws:sns:*:*:ActivatingLifecycleHookTopic-*",
                "arn:aws:sns:*:*:TerminatingLifecycleHookTopic-*"
            ]
        }
    ]
}
```

4. Set the name of policy as *FleetIQFiestaPolicy*.

![Fiesta](../../images/fleetiq/lab32/Fiesta-2[en].png)

5. Let's make IAM role to use this policy. Create Role with EC2 use case like below.

![Fiesta](../../images/fleetiq/lab32/Fiesta-3[en].png)

6. Attach *FleetIQFiestaPolicy* that we made right before to permission policies.

![Fiesta](../../images/fleetiq/lab32/Fiesta-4[en].png)

7. We should edit its trusted relationship. It is configured to ec2 for trusted entity.

![Fiesta](../../images/fleetiq/lab32/Fiesta-5[en].png)

8. Edit trusted relationship like below.

```json
{
    "Version": "2012-10-17",
    "Statement": [
       {
          "Effect": "Allow",
          "Principal": {
          "Service": [
              "gamelift.amazonaws.com",
              "autoscaling.amazonaws.com"
          ]
       },
       "Action": "sts:AssumeRole"
      }
    ]  
 }
```

* Policy document described above means GameLift and Auto Scaling can assume this role.


9. When you finish to make IAM policy and Role for Fiesta, let's move on configuration for server instances for FleetIQ.


---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
