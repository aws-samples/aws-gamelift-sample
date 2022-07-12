---
title: Making SQS
url: /awsservice/lab13
weight: 40
pre: "<b>1-3. </b>"
---


### Making SQS <br/><br/>

Let's make SQS configuration!    
We are using SQS queue for processing Game Result.    

1. Access to SQS menu on Console. https://console.aws.amazon.com/sqs
2. Click "Create Queue" button. Put queue name as "game-result-queue", and select Standard Queue. Click "Create Queue" button and finish creation.

![SQS](../../images/awsservice/lab13/SQS[en].png)

3. When queue creation is completed, check endpoint URL on its details. We are using this URL next part.

![SQS_Endpoint](../../images/awsservice/lab13/SQS_Endpoint[en].png)

We can check actions with Queue, DynamoDB, and ElastiCache on next part with Lambda functions.


---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
