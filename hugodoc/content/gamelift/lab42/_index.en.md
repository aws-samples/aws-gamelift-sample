---
title: Configuring API Gateway
url: /gamelift/lab42
weight: 30
pre: "<b>4-2. </b>"
---

{{% notice tip %}}
If you deploy CloudFormation stack, Lambda functions and API Gateway have already been made. You should put required part of these resources.
{{% /notice %}}

### Configuring API Gateway <br/><br/>

We know that one of our Lambda functions requires API Gateway. API Gateway provides endpoint for REST API, HTTP API and WebSocket API. It also has features like API caching, Throttling and operational benefits on API backend.

It plays a role of "Gateway" for Backend Lambda API on this Lab.

1. Move API Gateway menu on Console. (https://console.aws.amazon.com/apigateway)

2. Click "Create API", select REST and set its name "GomokuAPI". Endpoint should be **Regional**.

![APIGW](../../images/gamelift/lab42/APIGW_1[en].png)

3. Creating API, you can find empty page. Firstly, make Resource and create Method on it. Click "Actions" button and make Resource.

4. Put ranking as Resource Name, and **Enable API Gateway CORS**. Click "Create Resource".

![APIGW](../../images/gamelift/lab42/APIGW_2[en].png)

5. Next step is creating method. Click "Actions" button and Create Method. You can see small list box below. Select "GET" and click check buttons next to it.

6. Choose Integration Type **Lambda Function**, and right region. Select *game-rank-reader* as Lambda Function. Click Save. 

![APIGW](../../images/gamelift/lab42/APIGW_3[en].png)

7. API was made. Now we should deploy this API to production stage. Click "Actions" and "Deploy API".

8. Select [New Stage] and put Stage name "prod". Click Deploy button to proceed.

![APIGW](../../images/gamelift/lab42/APIGW_4[en].png)

9. After completion, you can check stage configuration like below. Please check this Invoke URL. We are using this URL as endpoint for S3 static web page.

![APIGW](../../images/gamelift/lab42/APIGW_5[en].png)

Nextly, we are making static web site built on S3. You can easily host static web site!

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
