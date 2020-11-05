---
title: Clean Up
url: /cleanup
weight: 70
pre: "<b>5. </b>"
---


{{% notice tip %}}
실습 과정 동안 고생하셨습니다..! 이제는 리소스들을 정리할 차례입니다.
{{% /notice %}}


### CloudFormation Clean Up 

CloudFormation 을 통해 리소스를 할당했다면, 스택을 삭제해서 쉽게 자동 구성된 리소스들을 삭제하실 수 있습니다.

![Clean](../images/cleanup/Clean-1.png)

이 때, 실습 과정에서 생성한 리소스에 Dependency 가 생겼다면 해당 Dependency 를 제거해주시거나 직접 해당 리소스를 제거해주셔야 합니다.

### 리소스 제거

콘솔에서 다음 리소스들을 제거해줍니다.

* GameLift 빌드, 플릿 및 별칭 구성
* GameLift 대기열, 매치메이킹 구성
* 매치메이킹 서버를 위한 EC2 인스턴스
* 유저 정보 처리를 위한 DynamoDB, ElastiCache, S3
* Lambda 함수
* API Gateway
* FleetIQ 실습을 위한 리소스


### FleetIQ 리소스 제거

FleetIQ 의 리소스 제거를 위해 CLIv2 환경이 설치된 위치에서 다음 커맨드를 실행합니다.

```sh
aws gamelift delete-game-server-group --game-server-group-name <게임 서버 그룹 이름>
```

---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
