---
title: SQS 생성하기
url: /awsservice/lab13
weight: 40
pre: "<b>1-3. </b>"
---


### SQS 생성하기 <br/><br/>

이제 SQS 설정을 시작합니다. SQS를 이용하여 게임 결과 처리를 위한 대기열을 만들 것입니다.

1. 콘솔에서 SQS 메뉴로 들어갑니다. https://console.aws.amazon.com/sqs
2. 대기열 생성을 클릭하여 생성을 시작합니다. Queue 이름은 game-result-queue로 입력하고, 표준 Queue를 선택합니다. 대기열 생성 버튼을 클릭하여 Queue 생성을 완료합니다.

![SQS](../../images/awsservice/lab13/SQS.png)

3. Queue 생성이 완료되면 Details에 보이는 endpoint URL을 기록해둡니다. 마찬가지로 뒤의 Lambda 소스 코드에 사용할 예정입니다.

![SQS_Endpoint](../../images/awsservice/lab13/SQS_Endpoint.png)

생성된 Queue와 함께 아까 생성된 Dynamo DB, ElastiCache의 동작은 뒤의 Lambda를 통하여 확인하도록 하겠습니다.



---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
