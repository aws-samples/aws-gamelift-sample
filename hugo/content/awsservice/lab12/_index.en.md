---
title: ElastiCache Configuration
url: /awsservice/lab12
weight: 30
pre: "<b>1-2. </b>"
---

### ElastiCache Configuration <br/><br/>

Nextly, we will configure ElastiCache. This cache will store ranking data.

1. Move to ElastiCache Console on your AWS Console. https://console.aws.amazon.com/elasticache
2. Create ElastiCache cluster. We are using Redis. Select Redis Cluster from the left menu and click Create Redis Cluster.
3. Fill the information like below.

![Redis](../../images/awsservice/lab12/ElastiCache_Redis01[en].png)
![Redis](../../images/awsservice/lab12/ElastiCache_Redis02[en].png)
![Redis](../../images/awsservice/lab12/ElastiCache_Redis03[en].png)
![Redis](../../images/awsservice/lab12/ElastiCache_Redis04[en].png)

* Name: gomokuranking    
* Engine: 6.2 (5.0.4 above recommended)    
* Port: 6379 (default)    
* Parameter group: default    
* Number of replicas: 0    

4. In subnet group setting, you can select an existing default vpc and subnet or create a new vpc and subnet group for workshop.

5. Click "Create" button to create "Redis". (It takes time to create Redis)

6. When Redis cluster turns it's status "Available", check Primary Endpoint. We use this endpoint on our Lambda functions.

![ElastiCache](../../images/awsservice/lab12/ElastiCache_DB[en].png)

7. Nextly, make Security Group for this ElastiCache. We are making very simple Security Group for our resources.

8. Let's move to VPC console(https://console.aws.amazon.com/vpc). Click "Security Group" menu on left side tab and create Security Group.

9. Put right information on Name tag, Group name, etc and Select default VPC.

10. We should fix inbound policy in this security group. Select Security Group and Click Inbound Rules.

11. Click "Edit" button, and create inbound policy like below. Put **Security Group ID itself as source**. In this policy, it is able to communicate between resources that have this security group.

![SecurityGroup](../../images/awsservice/lab12/SecurityGroup[en].png)

* Type: All traffic    
* Protocol: All    
* Source: Security Group itself    

12. After you finish creating Security Group, move to ElastiCache Console. Select Redis that we made.

13. Click "Modify" on the above, Modify Security Group settings to apply Security Group that we created before

![ElastiCache_SG](../../images/awsservice/lab12/ElastiCache_SG[en].png)

Now we finish ElastiCache configurations, next page is for SQS.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
