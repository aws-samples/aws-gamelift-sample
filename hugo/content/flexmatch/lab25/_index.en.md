---
title: Build Serverless FlexMatch
url: /flexmatch/lab25
weight: 60
pre: "<b>2-5. </b>"
---

### How to build Serverless FlexMatch<br/><br/>

{{% notice tip %}}
In this Lab, we are starting FlexMatch Matchmaking based on Serverless Architecture.
{{% /notice %}}

With FlexMatch, you can implement session based matchmaker easily without provisioning servers for it.
To use FlexMatch, you need to call FlexMatch API or SDK on game server or game client side.
In this Lab, we are using Lambda. This approach can allow cost-efficient serverless model on your game server backend.

### Configuring GameLift Queue

We are going to use GameLift Queue for matchmaking.

1. Click "Create a queue" on GameLift Console.
2. Put the name on queue like below. And choose Alias that we made before on destination. Click "Create queue" to make.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-1[en].png)

3. Nextly, we are making rule for FlexMatch. Click "Create matchmaking rule set" button.

4. Set rule set name as you want, and put rule set policy below on "Rule set box". Rule set is provided from **GomokuRuleSet.json** under the *rootFolder/Ruleset* directory on a given source code. Copy the code and paste it to console box.

```json
{
    "ruleLanguageVersion" : "1.0",
    "playerAttributes" :
    [
        {
            "name" : "score",
            "type" : "number",
            "default" : 1000
        }
    ],
    "teams" :
    [
        {
            "name" : "blue",
            "maxPlayers" : 1,
            "minPlayers" : 1
        },
        {
            "name" : "red",
            "maxPlayers" : 1,
            "minPlayers" : 1
        }
    ],
    "rules" :
    [
        {   "name": "EqualTeamSizes",
            "type": "comparison",
            "measurements": [ "count(teams[red].players)" ],
            "referenceValue": "count(teams[blue].players)",
            "operation": "="
        },
        {
            "name" : "FairTeamSkill",
            "type" : "distance",
            "measurements" : [ "avg(teams[*].players.attributes[score])" ],
            "referenceValue" : "avg(flatten(teams[*].players.attributes[score]))",
            "maxDistance" : 300
        }
    ],
    "expansions" :
    [
        {
            "target" : "rules[FairTeamSkill].maxDistance",
            "steps" :
            [
                {
                    "waitTimeSeconds" : 10,
                    "value" : 500
                },
                {
                    "waitTimeSeconds" : 20,
                    "value" : 800
                },
                {
                    "waitTimeSeconds" : 30,
                    "value" : 1000
                }
            ]
        }
    ]
}
```

This rule matches players that have score differences under 300, and if match cannot be accepted in time, mitigate matchmaking rule and find players!    
Click "Validate rule set", and check rule set is fine. And click "Create rule set".

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-2[en].png)

5. Next thing to do is linking GameLift Queue and Matchmaking rule set. Click "Create matchmaking configuration" on the menu. Select queue and rule set like below, and create configuration.

* Please check you choose right region and queue.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-3[en].png)


### Creating Lambda function

{{% notice tip %}}
If you deploy CloudFormation stack, Lambda functions have already been made. You should just put required part of these resources.
{{% /notice %}}

GameLift FlexMatch configuration was completed!    
Next step is building Lambda function and API Gateway that game client makes request for FlexMatch.
In this lab, we are going to make two Lambda functions. First one is handling matchmaking requests from client, second one is checking matchmaking results.    
Let's make first Lambda function. It will not be difficult!

1. Move to Lambda Console. https://console.aws.amazon.com/lambda

2. Click "Create function" button.

3. Check "Author from scratch".

4. Put the Name of the function "game-match-request".

5. Choose Python 3.9 as Runtime.

6. Choose Role on Permissions Tab. Check "Use an existing role" and select "Gomok-game-match-request" as IAM Role. Click "Create function" button.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-4[en].png)

7. Find **Lambda/MatchRequest.py** at your source code. Copy the contents and paste it to Console. Check Region and Match Configuration is configured correctly.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-5.png)

* Sample Image is just for reference. This can be different from your Lab environment. Also please check Handler and function name is same.

8. Set Basic settings on your function.
- Memory : 128MB
- Timeout : 1 min

9. Click "Save" button and make function. This function receives Matchmaking requests from game client, reads user data from DynamoDB and send Matchmaking requests to GameLift.

10. Click "Create function" button and let's make second function.

11. Check "Author from scratch" and start with empty function.

12. Put "game-match-status" as Name, and select Python 3.9 as Runtime.

13. Choose Role on Permissions Tab. Check "Use an existing role" and select "Gomok-game-match-status" as IAM Role. Click "Create function" button.

14. Find **Lambda/MatchStatus.py** at your source code. Copy source code and paste it to Lambda Code textbox.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-6[en].png)

15. Like above, set Basic settings on your function.
- Memory : 128MB
- Timeout : 1 min

{{% notice info %}}
Please check Lambda functions *Handler*. Lambda function can access this handler when it is executed.
{{% /notice %}}

16. Click "Save" button and make function. This function check matchmaking request is done by using TicketId that game clients sent, and send Game server's IP address and Port information to game client as response.


### API Gateway Configuration

Let's make API for these Lambda functions.

1. Move to API Gateway console. (https://console.aws.amazon.com/apigateway)

2. Choose API Gateway that is already made, and click "Create Resource" button.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-7[en].png)

3. Write Resource name *matchrequest* on the box. Check "Enable API Gateway CORS" and click "Create Resource" button.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-8[en].png)

4. Create new method for this resource. Click "Actions" button and "Create Method". Make API for "POST" method.

5. Select Integration type with Lambda function, and check correct region. Choose *game-match-request* for Lambda function and click "Save" button.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-9[en].png)

6. Make second API for second Lambda function. Click "Actions" on the root path and "Create Resource".

7. Create API as we had. Resource Name will be *matchstatus* and enable API Gateway CORS.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-12[en].png)

8. Making POST method, and integrate with *game-match-status* Lambda function. Click "Save" button.

9. Now we finish to make APIs for Lambda functions. Let's deploy this API. Click "Actions" button and "Deploy API".

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-10[en].png)

10. After you deploy your API, you can find Invoke URL. This URL will be used for game client.

![Flex-Match](../../images/flexmatch/lab25/FlexMatch-11[en].png)


---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

