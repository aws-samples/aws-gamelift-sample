---
title: 매치메이킹 이벤트 구성하기
url: /flexmatch/lab26
weight: 70
pre: "<b>2-6. </b>"
---

### FlexMatch 매치메이킹 이벤트 구성하기 <br/><br/>

다음으로 GameLift 서비스를 통해 트리거되는 매치메이킹 이벤트를 핸들링하는 기능을 만들어보겠습니다.     
GameLift FlexMatch Matchmaking Event 를 통해 Event Notification 을 전달받으면 GameLift API 를 Polling 하는 방식보다 빠르게 매치메이킹 구성을 전달받을 수 있으며 유연하게 사용자 세션을 관리할 수도 있습니다.     

이번 실습에서는 FlexMatch 매치메이킹 이벤트를 이용해서 Event Notification 방식으로 매치메이킹을 구성합니다.

1. Amazon SNS 콘솔로 이동합니다.

2. 왼쪽 메뉴에서 Topic(주제) 을 클릭합니다. Create Topic 버튼을 눌러서 새로운 주제를 생성합니다.

3. Topic Type 을 Standard 로 지정합니다. 이름은 다음과 같이 gomoku-match-topic 으로 지정해줍니다.

4. 하단의 Access Policy 를 수정합니다. Advanced 를 선택하고, JSON 기반 정책에 다음을 입력합니다. 아래 정책은 GameLift 서비스가 SNS 주제에 접근할 수 있도록 해줍니다.

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
위의 정책에서 {} 로 표기된 부분에는 해당하는 리전, Account 정보, 구성하는 Topic 이름(gomoku-match-topic)을 입력합니다.
Account 정보는 콘솔에서 우측 상단에 접속 아이디를 클릭하면 MyAccount 우측의 숫자 배열로 표시됩니다.
{{% /notice %}}

5. 나머지 설정은 그대로 두고 하단의 Create Topic 버튼을 눌러 생성합니다.

다음으로 이벤트를 핸들링할 Lambda 함수를 생성합니다. 이 함수는 위에서 생성한 Amazon SNS 를 구독해서 이전에 생성한 DynamoDB 에 연결 정보를 저장합니다.      

1. AWS Lambda 콘솔로 이동합니다.

2. Create Function 버튼을 클릭하여 다음 설정으로 새 Lambda 함수 생성을 시작합니다.
- Function Name : game-match-event
- Runtime : Python 3.9      
- Permissions > Execution Role : Create a new role from AWS policy templates
- Permissions > Role name : BasicLambdaDynamoDBRole
- Permissions > Policy templates 에서 Simple microservice permissions 을 선택

![Lambda](../../images/flexmatch/lab26/1.png)

3. 함수가 생성되면 Add Trigger 버튼을 눌러서 Amazon SNS 서비스를 선택합니다. 앞에서 만들어둔 SNS Topic(gomoku-match-topic) 을 선택합니다. 이 함수는 위에서 생성한 SNS Topic 을 Subscribe 하며, Publish 되는 메시지에 대해 트리거됩니다.

![Event](../../images/flexmatch/lab26/2.png)

4. 기존 함수 코드를 다음 코드로 변경해줍니다 (MatchEvent.py 파일을 참고합니다). 실습 환경에 알맞은 리전으로 세팅해줍니다.

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
함수 코드는 Amazon SNS 에서 트리거된 이벤트에서 매치메이킹 정보를 DynamoDB 로 저장합니다.
{{% /notice %}}

다음으로는 GameLift 가 SNS 로 이벤트를 트리거하도록 구성해봅니다.

1. Amazon GameLift 콘솔로 이동합니다.

2. Matchmaking Configuration 을 선택합니다. 기존에 만들어둔 GomokuMatchConfig 설정을 선택해줍니다.

3. Actions > Edit Matchmaking Configuration 버튼을 눌러 설정을 수정합니다.

4. 다음과 같이 Notification Target 에 앞서 만들어두었던 SNS Topic 의 ARN 을 입력해줍니다.

![Event](../../images/flexmatch/lab26/3.png)

{{% notice tip %}}
이제 GameLift 에서 구성된 매치는 SNS Topic 으로 바로 푸시됩니다. 해당 Topic 을 구동하는 Lambda 함수는 이를 받아서 DynamoDB 에 Connection 정보를 업데이트합니다.
{{% /notice %}}

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

