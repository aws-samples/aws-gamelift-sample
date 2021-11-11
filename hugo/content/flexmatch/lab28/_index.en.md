---
title: Analyzing Matchmaking Event
url: /flexmatch/lab28
weight: 90
pre: "<b>2-8. </b>"
---

### Analyzing FlexMatch Matchmaking Event <br/><br/>

When you make Game, executing itself will not be your goal. Operation and management are very important for Game Service.
FlexMatch provides Matchmaking Events, and it makes events related to GameLift tickets.

With Matchmaking events, we are able to use this features for checking issue status, match status monitoring or troubleshooting and debugging.

In this page, we are looking how to log Matchmaking events and analyze game session match patterns easily with FlexMatch Matchmaking events and CloudWatch Logs.

CloudWatch Logs is nice tool for monitoring Log streams near-realtime, ans we can make monitoring system by Logs Insight without provisioning any resources.    
So, it would be good opportunity to practice!

1. Creating Lambda function to handle FlexMatch Matchmaking events. Select Author from scratch. You don't need to change Permission settings for your comfort.

2. Set function name as *game-flexmatch-event*, and Runtime would be Python 3.9. Permission requires permission to record logs in CloudWatch Logs, and can be easily implemented with basic Lambda permission.

![Event](../../images/flexmatch/lab28/Event-1[en].png)

3. Function code is not provided, but you can use this source code below. :)

{{% notice tip %}}
If it is possible, I recommend you to modify this source code! Let's check how Matchmaking can be handled and what kinds of information can we get!
{{% /notice %}}

```python

import json

def lambda_handler(event, context):
    eventType = event['detail']['type']
    if (eventType == "MatchmakingSucceeded"):
        a = event['detail']['gameSessionInfo']['players'][0]['playerId']
        b = event['detail']['gameSessionInfo']['players'][1]['playerId']
        print("Matchmaking: " + a + "," + b)
    return {
        'statusCode': 200,
        'body': json.dumps('Hello from Lambda!')
    }

```

This code inspects Matchmaking event ticket types and write logs for Matchmaking data.
Logs printed in Lambda function send its output stream to CloudWatch Logs as default.

4. Access to CloudWatch Console in order to make trigger. (https://console.aws.amazon.com/cloudwatch)

5. Click "CloudWatch Events" on the left side of the console. Click "Get Started" or "Create Rule".

6. Let's create Rule like below. We could also configure various Targets such as SNS topic, but we are using Lambda functions(game-flexmatch-event) as Target.

![Event](../../images/flexmatch/lab28/Event-2[en].png)

7. Set proper name for event and click "Create" button. Check Rule was successfully created.

![Event](../../images/flexmatch/lab28/Event-3[en].png)

8. Let's play "Gomoku" game again! We can check CloudWatch Logs that CloudWatch Events logs events from Lambda functions. We also find Matchmaking events on logs. It contains Matchmaking lists on its logs.

![Event](../../images/flexmatch/lab28/Event-4[en].png)

9. Click "Logs Insight" on left side of menu.

10. Next queries provide Matchmaking results from log data as raw data filtering. 

```sql
fields @timestamp, @message
| filter @message like /Matchmaking:/
| sort @timestamp desc
| limit 20
```

You are able to put this query inside of textbox in Logs Insight

![Event](../../images/flexmatch/lab28/Event-5[en].png)

* The result can be different from picture above.


11. Next queries provide statistics information that is aggregated recent Matchmaking results.

```sql
stats count(*) by @message
| filter @message like /Matchmaking:/
| sort @timestamp desc
| limit 20
```

And result can be like below.

![Event](../../images/flexmatch/lab28/Event-6[en].png)

12. We have tested simple example this page. You can play more games, and make more queries for your game easily.
This kind of features will make you balance game session matching on early stage of your games.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

