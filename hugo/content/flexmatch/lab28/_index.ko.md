---
title: 매치메이킹 이벤트 분석하기
url: /flexmatch/lab28
weight: 90
pre: "<b>2-8. </b>"
---

### FlexMatch 매치메이킹 이벤트 분석하기 <br/><br/>

게임을 준비할 때, 게임이 작동하는 것 그 자체가 목적이 될 수는 없습니다. 그만큼 중요한 것이 바로 게임의 운영이죠.    
FlexMatch 는 매치메이킹 이벤트를 제공하며, GameLift 가 처리하는 티켓의 관련 이벤트들을 발생시킵니다.
이를 통해서, 새로 런칭한 게임의 매치메이킹이 버그없이 잘 동작하고 있는지, 올바른 상대와 적절하게 매칭이 되고 있는지 모니터링하실 수 있으며, 때로는 트러블슈팅 및 디버깅의 목적으로 사용하실 수도 있습니다.    

이번 실습에서는 FlexMatch 매치메이킹 이벤트를 이용해서 매치 티켓 정보를 로깅하고, CloudWatch Logs 를 통해서 세션 매칭 양상을 손쉽게 분석해보는 연습을 해보겠습니다.

CloudWatch Logs 는 로그에 대해 준실시간성으로 모니터링할 수 있는 간편한 도구이고, 이를 Logs Insight 를 통해서 별도의 리소스 구성 없이도 편하게 모니터링 시스템을 구축할 수 있습니다. 실습을 통해 연습해볼 수 있는 좋은 기회가 되겠습니다!

1. 먼저 FlexMatch 매치메이킹 이벤트를 처리할 Lambda 함수를 생성합니다. 바로 로그 그룹을 생성할 수도 있지만, Lambda 함수 구성을 통해 실습 도중 이벤트 데이터를 응용해서 사용해보시길 바랍니다.

2. Lambda 함수의 이름은 game-flexmatch-event 로 지정하시고, 런타임 환경은 Python 3.9 버전으로 구성합니다. Permission 으로는 CloudWatch Logs 에 로그를 기록할 수 있는 권한이 필요하며, 실습의 편의를 위해 기본 설정(basic Lambda permission)으로 쉽게 구현하실 수 있습니다.

![Event](../../images/flexmatch/lab28/Event-1.png)

3. 함수의 코드는 별도 파일로 제공되지는 않습니다. 다음 소스 코드를 복사 붙여넣기 하시고, 원하시는대로 가공하실 수 있습니다.

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

코드는 매치메이킹 이벤트의 티켓 타입을 검사하고, 매칭이 성공할 경우 매칭 정보를 로그로 기록합니다.    
Lambda 에서 작성된 Log 는 기본적으로 CloudWatch Logs 로 출력이 구성됩니다.

4. 다음으로 트리거 구성을 위해 CloudWatch 콘솔에 접속합니다. (https://console.aws.amazon.com/cloudwatch)

5. 좌측 메뉴의 CloudWatch Events 를 선택하고, "시작하기" 버튼 또는 "규칙 생성" 메뉴를 클릭합니다.

6. 다음과 같이 이벤트를 트리거하는 규칙을 생성합니다. 대상 서비스로 SNS 주제 등 다양한 서비스를 구성할 수 있지만, 본 실습에서는 조금 전에 만든 Lambda 함수(game-flexmatch-event)를 선택해줍니다.

![Event](../../images/flexmatch/lab28/Event-2.png)

7. 적당한 이름으로 이벤트를 다음과 같이 생성해주면 준비는 끝납니다.

![Event](../../images/flexmatch/lab28/Event-3.png)

8. 이제 다시 "오목" 게임을 플레이하도록 합니다. 게임을 플레이하면서 CloudWatch Logs 를 확인해보면 다음과 같이 CloudWatch Events 에서 Lambda 함수로 트리거되는 이벤트에 대한 정보가 로깅되는 것을 확인해보실 수 있습니다.

![Event](../../images/flexmatch/lab28/Event-4.png)

9. 다음으로 왼쪽 메뉴의 Logs Insight 메뉴를 클릭해줍니다.

10. 다음 쿼리는 로그 중에서 매치메이킹 결과를 raw data 형태로 필터링해서 제공합니다.

```sql
fields @timestamp, @message
| filter @message like /Matchmaking:/
| sort @timestamp desc
| limit 20
```

다음과 같은 결과를 확인해보실 수 있습니다.

![Event](../../images/flexmatch/lab28/Event-5.png)


11. 다음 쿼리는 최근 매치메이킹 결과에 대해서 집계해주는 통계 쿼리를 제공해줍니다.

```sql
stats count(*) by @message
| filter @message like /Matchmaking:/
| sort @timestamp desc
| limit 20
```

결과는 다음과 같습니다.

![Event](../../images/flexmatch/lab28/Event-6.png)


12. 실습에서는 간단한 예제를 따라해보았습니다. 게임을 더 플레이해보시고, 더 유용한 통계 쿼리를 쉽게 만들어보실 수 있습니다.    
이러한 이벤트 기능 등을 활용하면 게임 런칭 초반 세션 매칭 관련 밸런싱을 하는데 큰 도움을 줄 수 있습니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

