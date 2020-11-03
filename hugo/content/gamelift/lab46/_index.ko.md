---
title: 게임 클라이언트 구성 & 게임 플레이!
url: /gamelift/lab46
weight: 70
pre: "<b>4-6. </b>"
---

### 게임 클라이언트 구성 및 게임 플레이 <br/><br/>

{{% notice info %}}
이 단원의 내용은 FlexMatch 를 활용한 경우와 차이가 있습니다. FlexMatch 버전의 클라이언트 바이너리는 본 실습의 바이너리와 다름에 유의합니다. FlexMatch를 활용한 실습을 원한다면 2단원 실습을 해보실 수 있습니다.
{{% /notice %}}

앞선 실습에서 우리는 GameLift 서비스를 통해 세션을 처리할 게임 서버를 배치했고, 매치메이커 서버를 통해 배치된 GameLift 의 플릿으로 세션을 매칭시켜줄 수 있는 매치 서버 환경을 구성했습니다.    

이제 오목을 플레이할 때가 왔습니다! 와! 너무 신나는군요..!

실습을 위해서는 Binary 형태로 구성된 게임 클라이언트(Classic/client/GomokuClient.exe)를 이용하거나, 직접 루트 폴더의 GomokuClient 프로젝트를 Visual Studio 에서 빌드합니다. 직접 바이너리를 빌드하신다면 64비트 Release 모드로 빌드하시는 것을 권장드립니다.

1. GomokuClient 프로젝트의 루트 디렉토리에 있는 config.ini 파일을 수정합니다.

![Client](../../images/gamelift/lab46/Client-1.png)

이전 장에서 생성했던 매치메이커 서버의 EIP 를 설정하고, 연결할 포트를 설정합니다.
클라이언트는 별도의 로그인 기능을 지원하지 않으므로 환경 설정 파일에 로그인할 유저 정보를 입력합니다.
계정이 없다면 새로 회원 가입이 되겠지만, 로그인 정보가 맞지않다면 게임을 실행할 수 없으니 유의해주세요!

2. GomokuClient.exe 를 실행합니다. 게임 실행을 위해서는 **우클릭 한 뒤 Game Start 버튼을 클릭합니다.** 게임을 시작하면 매칭 상대를 탐색합니다.

3. 매칭이 되면 게임을 즐기실 수 있습니다! 상대를 꺾고 오목의 승자가 되어보세요!

![Client](../../images/gamelift/lab46/Client-2.png)

4. 게임 결과는 데이터베이스에 반영되며, 일전에 구성한 S3 웹사이트 엔드포인트를 통해 확인해보실 수 있습니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
