---
title: Service Information
url: /intro/lab03
weight: 35
pre: "<b>0-3. </b>"
---

{{% notice info %}}
Lab contains various AWS Services to make Full Stack Game Service.
This part guides some of services used on this lab.
{{% /notice %}}

### GameLift
* Amazon GameLift is managed AWS Service for Session based multiplayer games.    
* GameLift provides high performance and high reliability with cost efficiency.    
* GameLift serves managed hosting for Session based game, Realtime game, and independent FleetIQ features.    
[https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-intro.html](https://docs.aws.amazon.com/gamelift/latest/developerguide/gamelift-intro.html)


### DynamoDB
* Amazon DynamoDB is managed NoSQL database on AWS with high performance, scalability and reliability.     
* DynamoDB can be used efficiently on Serverless Architecture, and this database can handle high traffics on distributed environment.    
* In this lab, DynamoDB is used to store user data.    
[https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Introduction.html](https://docs.aws.amazon.com/amazondynamodb/latest/developerguide/Introduction.html)


### ElastiCache
* Amazon ElastiCache is managed In-memory database compatible with Redis and Memcached.    
* In this lab, we are using for storing ranking data.    
[https://docs.aws.amazon.com/AmazonElastiCache/latest/red-ug/WhatIs.html](https://docs.aws.amazon.com/AmazonElastiCache/latest/red-ug/WhatIs.html)


### SQS
* Amazon SQS provides high durability queue for system. It is able to use decouple on distributed systems with this queue for flexibility.    
* Game processes result with SQS queue.    
[https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/welcome.html](https://docs.aws.amazon.com/AWSSimpleQueueService/latest/SQSDeveloperGuide/welcome.html)


### Lambda
* AWS Lambda is serverless compute service that runs code without provisioning or managing servers.    
* We are going to use Lambda functions as API backend.
[https://docs.aws.amazon.com/lambda/latest/dg/welcome.html](https://docs.aws.amazon.com/lambda/latest/dg/welcome.html)


### API Gateway
* Amazon API Gateway provides API endpoints for REST API and WebSocket API, and serves various features.    
* In this lab, we are using Lambda functions with API Gateway.    
[https://docs.aws.amazon.com/apigateway/latest/developerguide/welcome.html](https://docs.aws.amazon.com/apigateway/latest/developerguide/welcome.html)


### S3
* Amazon S3 is Object Storage that we can use via Internet.    
* It is used for Static Web Hosting in this lab.     
[https://docs.aws.amazon.com/AmazonS3/latest/dev/Welcome.html](https://docs.aws.amazon.com/AmazonS3/latest/dev/Welcome.html)

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
