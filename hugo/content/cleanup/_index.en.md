---
title: Clean Up
url: /cleanup
weight: 70
pre: "<b>5. </b>"
---


{{% notice tip %}}
Congratulations! It is time to clean up your resources.
{{% /notice %}}


### CloudFormation Clean Up 

If you allocate resources using CloudFormation, it is able to remove resources deleting CloudFormation stack.

![Clean](../images/cleanup/Clean-0[en].png)

Click "Delete" button on CloudFormation console.

![Clean](../images/cleanup/Clean-1[en].png)

At this time, if there is a dependency on the resource created during the lab, you must either remove the dependency or manually remove the resource.

### Deleting Resources

Remove those resources on your console.

* GameLift Build, Fleet and Alias.
* GameLift Queue, Matchmaking configuration.
* EC2 instance for Matchmaker.
* DynamoDB, ElastiCache, S3.
* Lambda functions.
* API Gateway.
* Resources used for FleetIQ.


### Remove FleetIQ Resources

To remove resources from FleetIQ, execute the following command in the location where the CLIv2 environment is installed.    

```sh
aws gamelift delete-game-server-group --game-server-group-name <Game Server Group Name>
```

---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
