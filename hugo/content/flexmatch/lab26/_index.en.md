---
title: Configuring Matchmaking Event
url: /flexmatch/lab26
weight: 70
pre: "<b>2-6. </b>"
---

### Configuring FlexMatch Matchmaking Event <br/><br/>

In this lab, we are going to make a matchmaking event handler triggered by GameLift.    
Matchmaking event notification is a key feature of FlexMatch. It is able to track match making results quickly and flexibly.     

1. Move to the Amazon SNS Console. (https://console.aws.amazon.com/sns/v3/home)

2. Click "Topic" on the left menu. Click "Create Topic" button to create new Topic.

3. Set topic type to Standard. Make topic with name "gomoku-match-topic".

4. Modify the Access Policy below. Clicking the Advanced radio button, and put the JSON as below. This policy allows Amazon GameLift access to the SNS topic.     

```JSON
{
  "Version": "2008-10-17",
  "Id": "__default_policy_ID",
  "Statement": [
    {
      "Sid": "__default_statement_ID",
      "Effect": "Allow",
      "Principal": {
        "AWS": "*"
      },
      "Action": [
        "SNS:GetTopicAttributes",
        "SNS:SetTopicAttributes",
        "SNS:AddPermission",
        "SNS:RemovePermission",
        "SNS:DeleteTopic",
        "SNS:Subscribe",
        "SNS:ListSubscriptionsByTopic",
        "SNS:Publish",
        "SNS:Receive"
      ],
      "Resource": "arn:aws:sns:{your_region}:{your_account}:{your_topic_name}",
      "Condition": {
        "StringEquals": {
          "AWS:SourceOwner": "{your_account}"
        }
      }
    },
    {
      "Sid": "__console_pub_0",
      "Effect": "Allow",
      "Principal": { 
        "Service": "gamelift.amazonaws.com" 
      },
      "Action": "SNS:Publish",
      "Resource": "arn:aws:sns:{your_region}:{your_account}:{your_topic_name}"
    }
  ]
}
```

{{% notice info %}}
In the above policy, enter the relevant region, account information, and the topic name (gomoku-match-topic) in the part marked with {} in the above policy.
Account information is displayed as a numeric array on the right side of MyAccount by clicking the login ID in the upper right corner of the console.      
{{% /notice %}}

5. Click "Create Topic" button.

Next, create a Lambda function to handle the event. This function subscribes to the Amazon SNS topic created above and stores the connection information in the previously created DynamoDB.      

1. Move to the AWS Lambda console. (https://console.aws.amazon.com/lambda/home)

2. Click the Create Function button to start creating a new Lambda function with the following settings:
- Function Name : game-match-event
- Runtime : Python 3.9      
- Permissions > Execution Role : Create a new role from AWS policy templates
- Permissions > Role name : BasicLambdaDynamoDBRole
- Permissions > Policy templates > Simple microservice permissions

![Lambda](../../images/flexmatch/lab26/1.png)

3. When the function is created, click "Add Trigger" button on the left side, and select Amazon SNS. Choose SNS Topic created above (gomoku-match-topic). This function subscribes the Topic and is triggered with the given event.     

![Event](../../images/flexmatch/lab26/2.png)

4. Change the code as follows(Refer codes from MatchEvent.py) : 

```python
import boto3
import json
import time
import datetime

dynamodb = boto3.resource('dynamodb', region_name='us-east-1')
ddb_table = dynamodb.Table('GomokuPlayerInfo')

def lambda_handler(event, context):
    print(event)

    sns_message = json.loads(event['Records'][0]['Sns']['Message'])
    matchevent_status = sns_message['detail']['type']
    if matchevent_status == 'MatchmakingSucceeded':
        gamesession_info = sns_message['detail']['gameSessionInfo']

        address = gamesession_info['ipAddress']
        port = int(gamesession_info['port'])
        players = gamesession_info['players']
        
        for player in players:
            player_id = player['playerId']
            
            if 'playerSessionId' in player:
                player_session_id = player['playerSessionId']
                connection_info = { 'IpAddress': address, 'Port': port, 'PlayerSessionId': player_session_id, 'timestamp': int(time.time()), 'status': 'matching' }
                ddb_table.update_item(
                    TableName="GomokuPlayerInfo",
                    Key={ 'PlayerName' : player_id }, 
                    UpdateExpression="set ConnectionInfo = :connection_info",
                    ExpressionAttributeValues={
                        ':connection_info': "" + json.dumps(connection_info),
                    },
                    ReturnValues="UPDATED_NEW"
                )
    return {
        'statusCode': 200,
        'body': json.dumps('Hello from Lambda!')
    }
```

{{% notice tip %}}
The function code stores matchmaking information from events triggered by Amazon SNS to DynamoDB.     
{{% /notice %}}

Next, let's configure GameLift to publish events to the SNS topic.

1. Move to the Amazon GameLift console. (https://console.aws.amazon.com/gamelift/home)

2. Choose Matchmaking Configuration. Select the GomokuMatchConfig setting you made previously.

3. Editing the configuration by clicking Actions > Edit Matchmaking Configuration button.

4. Put SNS Topic's Arn created above at the Notification Target as follows :

![Event](../../images/flexmatch/lab26/3.png)

{{% notice tip %}}
Matches configured in GameLift are now pushed directly to social media topics. The Lambda function that drives the topic receives it and updates the connection information in DynamoDB.     
{{% /notice %}}

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

