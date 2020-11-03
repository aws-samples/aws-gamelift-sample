---
title: 게임 서버 그룹 만들기
url: /fleetiq/lab33
weight: 60
pre: "<b>3-3. </b>"
---

FleetIQ 는 **게임 서버 그룹**이라는 추상화된 게임 호스팅 계층을 만들고, 이를 Auto Scaling Group 과 연동합니다.    
게임 서버 리소스는 FleetIQ 에 등록되고 게임 서버 그룹 내에서 관리됩니다.    
FleetIQ 에서 게임 서버 그룹을 생성하면 Auto Scaling Group 이 생성되며, Auto Scaling Group 내에서 할당할 컴퓨팅 리소스가 시작됩니다.    
그렇기 때문에 컴퓨팅 리소스를 최적화하기 위하여 적절한 인스턴스 타입으로 구성된 게임 서버 그룹을 할당하는 것이 중요합니다.    
FleetIQ 는 예측 알고리즘에 기반하여 게임 서버 그룹 목록의 각 스팟 인스턴스 유형을 정기적으로 평가하여, 게임 호스팅에 대한 실행 가능성을 계산합니다.

### FleetIQ 인스턴스에서 사용할 IAM 역할 만들기
1. IAM 콘솔에 접속합니다. (https://console.aws.amazon.com/iam/home?)

2. 좌측 메뉴에서 역할을 선택하고 EC2 사용 사례로 새로운 역할을 생성합니다.

3. 권한 정책을 연결합니다. GameLift 로 검색하시면 관리형 정책인 GameLiftGameServerGroupPolicy 가 나타납니다.
앞서 실습을 진행하셨다면, 다음과 같이 GameLiftFullAccess 역할이 같이 포함되어 있습니다.

![GSR](../../images/fleetiq/lab33/GS-Role-1.png)

4. 두 정책을 선택하여 역할을 만들고 이름을 GameLiftFleetIQRole 으로 지정합니다.

### EC2 Launch Template 작성하기
1. EC2 콘솔에 접속합니다. (https://console.aws.amazon.com/ec2/v2/home)

2. 좌측 메뉴에서 시작 템플릿을 클릭하고, 새로운 시작 템플릿을 생성해줍니다.

3. 다음과 같이 템플릿을 구성합니다.

![GSG](../../images/fleetiq/lab33/GSG-1.png)

이전까지 Windows 기반의 실습을 진행했다면, 본 실습에서는 Linux 기반의 게임 서버를 구성할 예정이므로, Amazon Linux2 AMI 를 선택해줍니다.
로그인이 가능하도록 키-페어의 설정도 같이 해줍니다.

4. 기본 VPC 에서 LaunchTemplate 을 만들도록 합니다. VPC 설정 및 Security Group을 알맞게 구성하도록 합니다.
* 별도의 제약은 없지만, 서버와 클라이언트는 TCP 로 통신할 예정이므로, TCP 프로토콜에 적절한 인바운드 구성을 할 수 있도록 Security Group 을 구성합니다.

5. IAM 인스턴스 프로파일에서 바로 전 단계에서 구성한 GameLiftFleetIQRole 을 선택합니다.

6. 마지막으로 사용자 데이터에 다음을 입력합니다.

```sh
#!/bin/bash
sudo yum install python3 -y
sudo pip3 install boto3
sudo pip3 install requests
sudo yum install -y gcc-c++ make
curl -sL https://rpm.nodesource.com/setup_12.x | sudo -E bash
sudo yum install -y nodejs
```

위의 사용자 데이터 코드는 게임 서버가 생성되면 원격으로 Cloud9 IDE 를 연결하기 위한 작업을 나타냅니다.


### Game Server Group 생성하기

다음으로 Game Server Group 을 생성해봅니다. AWS CLIv2 가 설치된 환경에서 CLI 커맨드를 통해 이용하실 수 있습니다.

1. 다음과 같이 커맨드를 입력합니다.

```sh
aws gamelift create-game-server-group --game-server-group-name GameServerGroups --role-arn <FleetIQFiestaRole 의 arn> --min-size 1 --max-size 2 --launch-template LaunchTemplateId=<LaunchTemplate ID> --instance-definitions '[{"InstanceType": "c5.large"}, {"InstanceType": "m5.large"}]' --region ap-northeast-2
```

**위의 <> 괄호친 부분에 앞서 생성한 리소스들의 정보를 포함시켜야 합니다. 실습하고자하는 리전을 알맞게 입력했는지 확인합니다.**

2. CreateServerGroup 이 성공하면 다음과 같이 GameLift 콘솔에서 게임 서버 그룹을 확인해보실 수 있습니다.

![GSG](../../images/fleetiq/lab33/GSG-2.png)

게임 서버 그룹의 생성이 지나치게 지연된다면 권한을 확인해보시는 것이 좋습니다.

3. Auto Scaling Group 구성이 된 모습도 확인해보실 수 있습니다. 

![GSG](../../images/fleetiq/lab33/GSG-3.png)

EC2 콘솔에서 인스턴스가 자동으로 생성된 부분도 확인해보실 수 있습니다.    
게임 서버 그룹내에 가용한 게임 서버들입니다. 하지만 아직 해당 게임 서버들을 통해 FleetIQ 기능을 사용해보실 수는 없습니다.
FleetIQ 기능의 사용을 위해서는 FleetIQ 의 Lifecycle 을 구성해주셔야 합니다.


---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
