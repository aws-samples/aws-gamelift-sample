---
title: IAM 정책 및 역할 생성
url: /awsservice/lab14
weight: 50
pre: "<b>1-4. </b>"
---

### IAM 정책 및 역할 생성 <br/><br/>

1장의 마지막으로, 이번 실습에서 만드는 full stack application에 사용할 IAM 정책(policy)와 역할(role)을 생성합니다

1. 우선, 역할에 사용될 정책을 만듭니다. 이 랩에서는 편의를 위해서 AWS Managed Policy를 주로 사용합니다. 콘솔에서 IAM 메뉴로 이동합니다.
https://console.aws.amazon.com/iam

2. 메뉴에서 Policies를 선택하고 Create policy 버튼을 클릭합니다.

3. Visual editor에서 Service는 GameLift를 선택하고 Actions에서는 랩의 편의상 All GameLift actions를 선택합니다. Review Policy를 클릭하여 다음 단계로 진행합니다.

![IAM](../../images/awsservice/lab14/IAM_GameLift.png)
<img src="./images/IAM_GameLift.png" alt="drawing" style="width:200px;"/>

4. 정책의 이름(GameLiftFullAccess)을 입력하고 Create policy 버튼을 클릭합니다.

5. 다음은 후반부 Lamdba 함수에 사용할 5 가지 역할(Role)을 생성할 것입니다.

6. IAM 콘솔에서 Role 메뉴로 이동한 뒤 Create role 버튼을 클릭합니다.

7. Role type에서 AWS Service Role의 AWS Lambda를 선택합니다.

![IAM_Lambda](../../images/awsservice/lab14/IAM_Lambda.png)

8. 이 Role에는 3개의 정책을 할당할 것입니다. 순서대로 AmazonSQSFullAccess, AmazonDynamoDBFullAccess, AWSLambdaBasicExecutionRole 정책을 선택합니다.

9. Tag는 선택 사항으로 입력합니다. Next: Review를 선택합니다. Role name은 Gomok-game-sqs-process를 입력합니다.

10. Create role 버튼을 클릭하여 첫 번째 역할 생성을 완료합니다.

11. 두 번째 역할도 첫 번째와 동일한 방법으로 생성합니다. 하지만 이번에는 AmazonDynamoDBFullAccess, AmazonVPCFullAccess, AWSLambdaBasicExecutionRole 정책을 추가합니다.

12. 두 번째 역할의 이름은 Gomok-game-rank-update로 지정합니다.

13. 세 번째 Role도 동일한 방법으로 생성합니다. 이번에는 AmazonVPCFullAccess, AWSLambdaBasicExecutionRole 정책을 추가해줍니다.

14. 세 번째 역할의 이름은 Gomok-game-rank-reader로 지정합니다.

15. 네 번째 역할도 첫번째와 동일한 방법으로 생성합니다. 이번에는 AWSLambdaBasicExecutionRole, AmazonDynamoDBFullAccess, 그리고 앞에서 생성한 GameLiftFullAccess 정책을 추가해 줍니다.

16. 네 번째 역할의 이름은 Gomok-game-match-request로 지정합니다.
17. 마지막으로 다섯 번째 역할도 동일한 방법으로 생성합니다. 이번에는 AWSLambdaBasicExecutionRole과 앞에서 생성한 GameLiftFullAccess 정책을 추가해 줍니다.
18. 다섯 번째 역할의 이름은 Gomok-game-match-status로 지정합니다.
19. 모두 생성이 완료되었다면 다음 스크린 캡처와 같이 다섯개의 역할을 확인할 수 있습니다.

![Gomok_Roles](../../images/awsservice/lab14/Gomok_roles.png)

20. 마지막으로 하나의 역할을 더 생성합니다. 앞서는 Lambda가 사용할 역할을 생성하였지만, 이번에는 GameLift에서 생성하는 Fleet에서 사용할 역할을 생성합니다. GameLift의 Fleet에서는 Game의 결과를 SQS에 전송할 때 사용하는 권한이 필요합니다.

21. Create Role을 선택하여 새로운 역할을 생성합니다. 이 역할의 사용은 GameLift 서비스가 수행하므로 GameLift를 서비스를 선택해야 하지만, 콘솔 환경에 따라 해당 Use case 에 대해 우리가 사용하고자하는 정책을 할당하지 못할 수 있습니다. 따라서 EC2 를 선택하고 다음으로 이동합니다.

22. 이 역할이 필요한 권한은 AmazonSQSFullAccess를 선택하고 역할 이름은 Gomoku-GameLiftFleetRole로 입력합니다.

![GomokFleetRole](../../images/awsservice/lab14/GomokFleetRole.png)

23. 이 역할을 사용할 수 있는 서비스를 GameLift로 변경해야 합니다. 앞서 생성한 역할을 선택하여 Trust relationships 탭을 선택하고, Edit trust relationship 버튼을 클릭합니다.
24. 아래와 같이 ec2로 되어 있던 부분을 gamelift로 변경하고 update trust policy 버튼을 클릭하여 저장합니다.

![GameLiftAssumeRole](../../images/awsservice/lab14/GameLiftAssumeRole.png)

여기까지가 1장 실습의 내용입니다. :)    
다음 장에서는 본격적으로 게임에 들어가는 로직을 구현해보겠습니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
