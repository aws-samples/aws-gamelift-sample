---
title: Creating Matchmaker Server
url: /gamelift/lab45
weight: 60
pre: "<b>4-5. </b>"
---

### Create server instance for Matchmaking <br/><br/>

{{% notice tip %}}
The content of this section is different from the case of using FlexMatch. FlexMatch uses its own matchmaking provided by GameLift, without making your own matchmaker. If you want to practice using FlexMatch, you can try [Lab 2](../../flexmatch).
{{% /notice %}}

If a game server is being deployed on Fleet that will handle real game sessions and run logic, you need a matchmaker to properly establish a TCP connection to that server.    

In general, a session-based game is implemented in such a way that the server for matchmaking is handled by a separate instance, and the server distributes the session to the game server.

In this Lab, you are able to use Matchmaker server binary(Matchmaker.exe) or build GomokuMatchMaker project on Visual Studio by yourselves.

1. Modify config.ini included with executable matchmaker(.exe).

![Matchmaker](../../images/gamelift/lab45/Match-1.png)

Specify the Alias ​​created earlier as above, and configure the port settings and service region appropriately.
Configure the DynamoDB table name that contains user information also.

2. Next, you need to create an IAM Role for the EC2 server where the matchmaker will be configured.

![Matchmaker](../../images/gamelift/lab45/Match-2[en].png)

Creating EC2 IAM Role, and grant access control policies for DynamoDB and GameLift.

3. Configure the EC2 server to act as a matchmaker.

![Matchmaker](../../images/gamelift/lab45/Match-3[en].png)

- Configure an instance based on Windows 2016. This binary was written and tested on 2016 basis.
- Assign a security group that allows the port specified in config.ini (default 5999). That policy should be allowed for TCP traffic.
- Give the instance the IAM Role created in the above step.

4. Access to the instance, copy Matchmaker binary. Launching **vc_redist_x64.exe** for the first time, configure C++ runtime environment.

5. Once the environment is configured, run GomokuMatchMaker.exe to run Matchmaker.

6. Finally, assign an Elastic IP (EIP) address and give it to your instance so that you can contact the Matchmaker server.
GomokuClient will connect to the matchmaker server with that IP address!

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
