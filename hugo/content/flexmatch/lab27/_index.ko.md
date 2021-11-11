---
title: FlexMatch 클라이언트와 연결
url: /flexmatch/lab27
weight: 80
pre: "<b>2-7. </b>"
---

### FlexMatch 를 이용한 게임 클라이언트와 연동 <br/><br/>

FlexMatch 기능을 사용하기 위해서는 서버 또는 클라이언트 레벨에서 FlexMatch 기능을 사용하도록 일부 바이너리의 수정이 필요합니다.
이번 실습에서는 게임 클라이언트가 조금 전에 우리가 만든 Lambda 함수를 호출하고, Lambda 함수가 서버리스 형태로 FlexMatch 기능을 통해 클라이언트의 세션을 적합한 게임 서버로 매칭시켜 줍니다.

우리는 Windows OS 에서 동작하는 오목 클라이언트를 사용할 것입니다. 

게임 클라이언트는 Outbound TCP 커넥션을 2개 맺습니다. (방화벽 설정이 필요하다면 설정해줍니다.)

1. 다운로드 받은 실습 파일에서 bin/FlexMatch/Client_player1 폴더로 이동합니다.

2. 해당 Client_player1 폴더 내의 config.ini 파일의 수정이 필요합니다.

3. 앞선 Section 5의 마지막 단계에서 확인한 API Gateway URL을 MATCH_SERVER_API값에 입력합니다.

![CC](../../images/flexmatch/lab27/CC-1.png)

4. PLAYER_NAME과 PLAYER_PASSWD를 (임의로) 지정합니다.

5. 게임 클라이언트(GomokuClient.exe)를 실행합니다. 아래의 스크립 캡처와 같이 실행될 것입니다.

6. 다른 플레이어를 실행해야 합니다. 다운로드 받은 실습 파일에서 bin/FlexMatch/Client_player2 폴더로 이동하여 config.ini 파일을 앞서와 같이 수정하고 player2를 실행합니다.

7. 두 개의 게임 화면에서 우클릭한 뒤 Start 하세요!

8. 이미 졌다고 판단된다면 우클릭한 뒤 Give up을 할 수 있습니다.

9. DynamoDB의 Score를 임의로 변경하여 Matchmaking을 테스트해 보십시요

![CC](../../images/flexmatch/lab27/CC-2.png)

GameLift 및 FlexMatch 를 활용한 풀스택 게임 서버 구축 단계는 여기까지입니다.

이후 실습 과정 및 FleetIQ 를 통해 GameLift 의 Advanced 한 내용들에 대해 학습하실 수 있습니다. :)


---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

