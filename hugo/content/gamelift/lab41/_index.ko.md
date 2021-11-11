---
title: Lambda 함수 구성하기
url: /gamelift/lab41
weight: 20
pre: "<b>4-1. </b>"
---


{{% notice tip %}}
CloudFormation 스택을 배포하셨다면, 이미 Lambda 함수와 API Gateway 의 기본 틀은 마련되어 있습니다. 단계를 진행해보시면서 필요한 부분만 채워주시면 됩니다.
{{% /notice %}}


### Lambda 함수 생성하기 <br/><br/>

1장에서 우리는 full stack game의 가장 기초적인 서비스들을 구성했습니다. 지금부터는 사용자가 게임을 즐기고 게임 결과를 처리할 Lambda 를 구성하겠습니다.

이번 랩에서는 총 3개의 Lambda 함수를 생성할 것입니다.
먼저 첫 번째 Lambda 함수를 작성하는 방법입니다.

1. 콘솔에서 Lambda 메뉴로 이동합니다. https://console.aws.amazon.com/lambda

2. Create function 버튼을 클릭하여 첫번째 함수 생성을 시작합니다.

3. Author from scratch 메뉴를 선택하여 빈 함수를 우선 생성합니다.

4. Name 항목에는 game-sqs-process를 입력합니다.

5. Runtime은 Python 3.9을 선택합니다.

6. Permissions 항목에서 Role 은 Use an existing role을 선택하고 기존에 만들어둔 Gomok-game-sqs-process를 선택하고 Create function을 실행합니다.

7. 생성이 완료되면 Designer그룹의 Add triggers 하단에 SQS(Simple Queue Service)를 Lambda의 실행 트리거로 선택합니다. 그러면 하단에 Configure triggers 그룹이 생성됩니다.

![Lambda](../../images/gamelift/lab41/Lambda-1.png)

8. SQS queue는 앞서 생성한 SQS의 arn이 선택되어 있는 것을 확인합니다. Enable trigger가 선택된 것을 확인하고 Add 버튼을 선택합니다.

Advanced settings: 128MB Memory and 1 min timeout

![Lambda](../../images/gamelift/lab41/Lambda-2.png)

9. Designer 그룹으로 돌아와 아래의 생성할 함수의 정보를 참고하여 Lambda 함수를 작성합니다. 소스 경로 최상위의 Lambda 폴더를 보면 GameResultProcessing.py 함수 내용을 참고해서 Lambda 함수를 작성합니다.

Code: GameResultProcessing.py 파일의 내용을 Copy&Paste합니다. 코드 내의 region_name 부분은 여러분이 랩을 수행하는 리전으로 되어 있는지 확인합니다. (예: ap-northeast-2)

![Lambda](../../images/gamelift/lab41/Lambda-3.png)

10. Save 버튼을 클릭하여 함수를 생성합니다. 생성한 함수는 SQS 에 기록된 게임 결과 점수를 읽어와 DynamoDB 에 업데이트하는 역할을 수행합니다. Lambda 함수는 SQS 대기열을 지속적으로 폴링하며 메시지를 가져와 DynamoDB 에 점수 결과를 반영합니다.

**실습의 편의를 위하여 Python Package 를 별도의 바이너리 형태로 제공합니다. 즉, GetRank.py 와 Scoring.py 를 이용한 Lambda 함수 작성은 제공받은 바이너리 안에 Lambda/LambdaDeploy 경로를 참고합니다.**

두 번째 Lambda 함수는 Python 패키지를 업로드하여 생성합니다.    이전에 생성한 함수는 Python 표준 SDK만을 사용하기 때문에 인라인 편집기를 사용했지만, 다른 Lambda 함수들은 Redis 라이브러리를 참조하기 때문에 배포 패키지를 업로드 하여 함수를 생성할 것입니다. Lambda 함수를 Python 배포 패키지를 업로드하여 생성해볼 수 있는 아주 좋은 기회입니다.

소스 코드를 작성(수정)한 후 배포 패키지 형태로 묶은 다음(LambdaDeploy.zip 파일) 이를 업로드하여 Lambda 함수를 생성할 것입니다. (만약 배포 패키지를 어떻게 직접 만드는지 알고 싶으시다면 Appendix A를 참조하세요.)

1. Lambda 폴더 하위에 LambdaDeploy 폴더에 GetRank.py와 Scoring.py라는 두 개의 Python 파일이 보일 것입니다.

2. 두 개의 파일 모두 저장한 뒤, 다시 LambdaDeploy.zip으로 압축해줍니다. (참고: “LambdaDeploy”폴더가 압축파일에 포함되면 안됩니다. 즉, GetRank.py 및 Scoring.py파일은 압축파일 내의 루트경로에 있어야 합니다.)

3. 배포 패키지를 완성하면 이를 이용해서 Lambda 함수를 생성할 수 있습니다.

4. 앞 서와 동일하게 Author from scratch 메뉴를 선택하여 함수 생성을 시작합니다.

5. Name은 game-rank-update으로 지정하고, Runtime은 Python 3.9로 선택하고, Role은 Gomok-game-rank-update을 선택하고 Create function을 누릅니다.

6. Function code그룹에서 Code entry type을 Upload a .ZIP file로 선택하고 LambdaDeploy.zip을 업로드합니다.

7. Handler 항목에서는 Scoring.handler 를 입력하고, 환경 변수에 다음과 같이 ElastiCache Endpoint 를 입력해줍니다.    
이와 같이 Lambda 의 환경변수를 이용하면, 안전하게 엔드포인트를 관리할 수 있고, 소스에 하드코딩할 필요없이 효율적으로 관리할 수 있습니다.

![Lambda](../../images/gamelift/lab41/Lambda-4.png)

8. 하단의 기본 설정 그룹에서 Timeout을 1분으로 변경합니다

9. Network 항목에서 VPC를 기존에 생성한 ElastiCache가 있는 VPC로 선택합니다. 서브넷 항목은 모두 선택하여 주시고, Security Group은 이전에 생성했던 GomokuDefault로 선택하여 주시기 바랍니다

![Lambda](../../images/gamelift/lab41/Lambda-5.png)

10. 상단의 Designer 그룹의 Add triggers에서 DynamoDB을 선택합니다.

![Lambda](../../images/gamelift/lab41/Lambda-6.png)

11. 하단에 Configure triggers그룹에서 GomokuPlayerInfo DynamoDB 테이블을 Trigger 테이블로 사용할 것입니다. 다른 부분은 기본값을 유지하고 Starting position은 수평 트리밍을 선택합니다. Enable trigger를 체크한 뒤 Add 버튼을 클릭합니다.

![Lambda](../../images/gamelift/lab41/Lambda-7.png)

12. 위의 과정이 완료되면 오른쪽 상단의 Save를 눌러 함수 생성을 완료합니다. (이 단계에서 함수 Test를 누르게 되면 아직 실패합니다.)


마지막으로 세번째 Lambda 함수를 생성하겠습니다. 두번째 만든 함수와 동일하게 만들 수 있습니다.

1. 세 번째 함수도 Author from scratch 를 선택하여 생성합니다.

2. Name항목에서 game-rank-reader를 입력하고, Runtime을 Python 3.9로 설정하고 Role을 이전에 만들어둔 Gomok-game-rank-reader를 선택합니다.

3. Create function을 누른 후 Code entry type에서 Upload a .ZIP file을 선택하여 LambdaDeploy.zip을 업로드합니다.

4. Handler 부분에서 GetRank.handler를 입력합니다. 역시 환경 변수에 좀 전과 같이 ElastiCache Endpoint 를 입력해줍니다.   

5. Basic settings 부분에서 Timeout을 1 min으로 설정합니다. Network 부분에서 이전 함수 동일하게 VPC 를 기존에 생성한 ElastiCache 가 있는 VPC 로 선택합니다. subnet 항목은 모두 선택하여 주시고, Security Group 은 이전에 생성했던 GomokuDefault 로 선택하여 주시기 바랍니다.

6. 이제 Save 를 눌러 Lambda 함수를 생성합니다.

여기까지 완료했다면 3 개의 Lambda 함수가 생성된 것을 확인하실 수 있습니다.

![Lambda](../../images/gamelift/lab41/Lambda-8.png)

이제 모든 Lambda 함수는 준비되었습니다. 지금부터는 이 함수를 실행할 방법이 필요합니다. 처음 생성한 두 함수는 SQS와 DynamoDB에 의해 호출될 것입니다.    
하지만 마지막 함수는 trigger를 설정하지 않았습니다. 어떻게 해야 해당 함수를 호출할 수 있을까요? 여기에서 API Gateway를 사용할 것입니다. 이 부분은 다음 섹션에서 내용이 이어집니다.

---

#### 함수 흐름 테스트 <br/><br/>


여기까지 준비가 되면 Section 1과 2에서 만들어진 여러 요소들이 어떻게 동작하는지를 테스트해 볼 수 있습니다. 간략하게 설명하면 전체 흐름을 보면 SQS에 데이터가 삽입되면 만들어진 Lambda함수가 실행되면서 해당 SQS데이터를 DynamoDB에 업데이트합니다. Dynamo DB에 입력된 데이터는 DynamoDB Stream의 Trigger를 통하여 다른 Lambda함수가 실행되어 Redis Cache가 업데이트 되는 형태를 가지게 됩니다. 

간단하게 Redis Cache의 내용을 확인할수는 없으나 전체 흐름이 정상적으로 동작하는지 여부를 테스트 데이터의 입력과 CloudWatch를 통하여 확인해보도록 하겠습니다.
1. Console을 통하여 SQS를 열어줍니다. https://console.aws.amazon.com/sqs
2. 위에서 생성한 game-result-queue를 선택하고 화면 위의 Queue Actions버튼을 눌러줍니다.
3. 나오는 메뉴 중에서 Send Message를 선택합니다.
4. 나오는 텍스트 상자에 아래의 JSON을 넣어줍니다. (특수 기호 때문에 붙여 넣기보다는 직접 입력을 추천합니다. 대소문자 구분에 주의하십시요. 정상 동작하지 않을 경우, CloudWatch Logs에 남아 있는 lambda 출력을 확인하세요.)
{ "PlayerName" : "SomeName" , "WinDiff" : 1, "LoseDiff" : 0, "ScoreDiff" : 100 }

5. 그리고 Send Message버튼을 눌러줍니다.
6. 이제 DynamoDB콘솔로 옮겨가서 GomokuPlayerInfo 테이블을 열어줍니다. Items 탭을 보면 위에서 넣어준 JSON데이터에 해당하는 항목이 반영되어 있는 것을 확인 할 수 있습니다. (이미 같은 PlayerName에 해당하는 항목이 있다면 Win이 1증가하고 Score가 100증가했을 것입니다.)

* 만약 반영이 되지 않을 경우 CloudWatch 를 통해 메시지 처리 내역을 확인해볼 수 있습니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
