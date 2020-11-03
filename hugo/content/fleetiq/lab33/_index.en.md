---
title: Creating Game Server Group
url: /fleetiq/lab33
weight: 60
pre: "<b>3-3. </b>"
---

FleetIQ makes abstraction layer **Game Server Group** for game hosting, and it uses Auto Scaling Group.
Game server resources are registered in FleetIQ, and managed in Game Server Group.

When you create a game server group in FleetIQ, an Auto Scaling Group is created, and computing resources to allocate within the Auto Scaling Group are launched.
So it's important to allocate a group of game servers with appropriate instance types to optimize your computing resources.
FleetIQ regularly evaluates each Spot instance types in the game server group list based on the prediction algorithm to calculate the feasibility for hosting the game.

### Making IAM Role for FleetIQ Instances
1. Move to IAM console. (https://console.aws.amazon.com/iam/home?)

2. Click "Role" button on IAM console, and create new Role for EC2 use case.

3. Attach Permissions policies. If you search "GameLift" as a keyword, you are able to find GameLiftGameServerGroupPolicy.
Also if we finish previous lab, you can find GameLiftFullAccess Role. (If not, please refer 1-4 on this lab)

![GSR](../../images/fleetiq/lab33/GS-Role-1[en].png)

4. Select two policies, make Role, and set its name GameLiftFleetIQRole.

### Creating EC2 Launch Template
1. Access to EC2 console (https://console.aws.amazon.com/ec2/v2/home)

2. Click Start Template in the left menu and create a new start template.

3. Configure the template as follows.

![GSG](../../images/fleetiq/lab33/GSG-1[en].png)

If you have conducted Windows-based labs before, in this lab, we will configure a Linux-based game server, so choose Amazon Linux2 AMI.
This can show you how your architecture can be flexible.
The key-pair setting is also performed to enable login.

4. Create a LaunchTemplate in your default VPC. Make sure to configure the VPC settings and security groups accordingly.
* There are no restrictions, but since the server and the client are going to communicate via TCP, the Security Group is configured to allow proper inbound configuration for the TCP protocol.

5. Choose **GameLiftFleetIQRole** that we made previously for IAM instance Profile.

6. Put follow text as user data.

```sh
#!/bin/bash
sudo yum install python3 -y
sudo pip3 install boto3
sudo pip3 install requests
sudo yum install -y gcc-c++ make
curl -sL https://rpm.nodesource.com/setup_12.x | sudo -E bash
sudo yum install -y nodejs
```

The user data code above describes the operation to connect the Cloud9 IDE remotely when the game server is created.


### Creating Game Server Group

Let's make Game Server Group. You are able to try this with AWS CLI v2.

1. Put command as follows.

```sh
aws gamelift create-game-server-group --game-server-group-name GameServerGroups --role-arn <FleetIQFiestaRole ARN> --min-size 1 --max-size 2 --launch-template LaunchTemplateId=<LaunchTemplate ID> --instance-definitions '[{"InstanceType": "c5.large"}, {"InstanceType": "m5.large"}]' --region ap-northeast-2
```

{{% notice info %}}
Please put right information between brackets <>. Also check for region on this command.
{{% /notice %}}

2. When you finish "CreateServerGroup", you are able to see Game Server Group that you made on GameLift Console as follows.

![GSG](../../images/fleetiq/lab33/GSG-2[en].png)

If the creation of the game server group is too delayed, it is recommended to check the permissions.

3. You can check Auto Scaling Group configurations.

![GSG](../../images/fleetiq/lab33/GSG-3[en].png)

You can also check the auto-created instance in the EC2 console.
Those indicate game servers available in the game server group.

However, you can't use FleetIQ function through the game servers yet.
To use FleetIQ function, you must configure FleetIQ Lifecycle.


---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
