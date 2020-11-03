---
title: GameLift Classic Lab (Option)
url: /gamelift
weight: 60
pre: "<b>4. </b>"
---

{{% notice info %}}
This Lab will implement "Gomoku" game using purely GameLift function regardless of FlexMatch or FleetIQ. If the FlexMatch feature is not yet available in some regions, this lab can be useful for getting started with GameLift.
{{% /notice %}}

### Lab Index
**1. Making Lambda functions for processing Game Result.**    
**2. Creating API Gateway for ranking board.**    
**3. Making S3 based static web hosting.**    
**4. Create Game Server Build and Fleet.**    
**5. Create EC2 instance for Matchmaking.**    
**6. Configuring Game Client and connect Session Game Server**    


### Architecture Diagram

![Architecture](../images/gamelift/Architecture.png)

The difference from the FlexMatch based architecture is that it configures **independent instances for matchmaking**. We will focus on this part and practice. There is no significant difference in the rest of the process.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
