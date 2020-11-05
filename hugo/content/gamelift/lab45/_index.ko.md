---
title: 매치메이커 서버 만들기
url: /gamelift/lab45
weight: 60
pre: "<b>4-5. </b>"
---

### 매치메이킹을 위한 서버 구축 <br/><br/>

{{% notice tip %}}
이 단원의 내용은 FlexMatch 를 활용한 경우와 차이가 있습니다. FlexMatch 는 직접 매치메이커를 만들 필요 없이 GameLift 에서 제공하는 자체 매치메이킹을 이용합니다. FlexMatch를 활용한 실습을 원한다면 2단원 실습을 해보실 수 있습니다.
{{% /notice %}}

실제 게임 세션을 처리하고 로직을 작동시킬 게임 서버가 Fleet 위에 배포되고 있다면, 해당 서버에 적절하게 TCP 연결을 맺어줄 매치메이커가 필요합니다.     
일반적으로 세션 기반 게임은 매치메이킹을 위한 서버는 별도의 인스턴스가 담당하고, 해당 서버가 세션을 게임 서버에 분산해주는 방식으로 구현됩니다.    

실습을 위해서는 Binary 형태로 구성된 매치메이커 서버(Matchmaker.exe)를 이용하거나, 직접 루트 폴더의 GomokuMatchMaker 프로젝트를 Visual Studio 에서 빌드합니다.

1. 실행 파일(.exe) 과 같이 포함된 config.ini 파일을 수정합니다.

![Matchmaker](../../images/gamelift/lab45/Match-1.png)

위와 같이 좀 전에 생성한 Alias 를 지정하고, 포트 설정과 서비스 리전을 알맞게 구성해줍니다.
사용자 정보가 포함된 DynamoDB 테이블 이름도 구성해줍니다.

2. 다음으로 매치메이커가 구성될 EC2 서버를 위한 IAM Role 을 생성해야 합니다.

![Matchmaker](../../images/gamelift/lab45/Match-2.png)

EC2 IAM Role 을 생성하고, DynamoDB 와 GameLift 에 대한 접근 제어 정책을 부여합니다.

3. 매치메이커 역할을 담당할 EC2 서버를 구성합니다.

![Matchmaker](../../images/gamelift/lab45/Match-3.png)

- Windows 2016 기반의 인스턴스를 구성합니다. 본 바이너리는 2016 기반에서 작성되고 테스트되었습니다.
- config.ini 에 명시했던 포트(기본값 5999)를 허용하는 Security Group 을 할당합니다. 해당 정책은 TCP 트래픽에 대해 허용되어야 합니다.
- 위의 단계에서 생성한 IAM Role 을 인스턴스에 부여합니다.

4. 해당 인스턴스에 접속하고, Matchmaker 바이너리를 복사합니다. vc_redist_x64.exe 를 최초 1회 실행하여, C++ 실행 환경을 미리 구성합니다.

5. 환경이 구성되었으면 GomokuMatchMaker.exe 를 실행시켜 매치메이커를 작동시킵니다.

6. 마지막으로 매치메이커 서버에 접속할 수 있도록 Elastic IP(EIP) 주소를 할당하고 인스턴스에 부여합니다.
GomokuClient 가 해당 IP 주소로 매치메이커 서버에 연결할 것입니다!

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
