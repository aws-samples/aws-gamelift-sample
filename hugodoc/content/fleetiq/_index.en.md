---
title: Start with FleetIQ
url: /fleetiq
weight: 50
pre: "<b>3. </b>"
---

### Start Independent FleetIQ features on your Game.

Previously, we made session game server instances using GameLift. GameLift provided us Spot fleet for our game server instances.
With this "SPOT fleet", you are able to save 60-80% costs for the instances. And GameLift feature named "FleetIQ" suggested the most reliable instances for SPOT instances against Spot Interruption. So, that is the reason why we are able to use GameLift with cost-efficienty and reliability.

**This feature "FleetIQ" is now supported independently.** Now we can manage EC2 instances and Auto Scaling Group by ourselves and make flexible architecture using SPOT instances. Therefore, we can have cost-efficient, more reliable and flexible architecture for our game server!

{{% notice info %}}
In this part, Let's try how to start FleetIQ easily.
Independent FleetIQ feature is currently provided as Preview, so this material will be updated continously.
{{% /notice %}}

### Architecture Diagram

Although FleetIQ feature is included in GameLift, it has independent architecture.

![FleetIQ](../images/fleetiq/FleetIQ.png)

{{% notice tip %}}
Game server and client can be implemented using FleetIQ supported SDK, CLI or API.
It uses independent from GameLift, so it does not depend on GameLift lifecycle.
{{% /notice %}}

Let's take a look how it works!

For more details about FleetIQ, you can refer link below.

[https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/gsg-intro.html](https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/gsg-intro.html)


---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
