---
title: GameLift Classic 실습 (옵션)
url: /gamelift
weight: 60
pre: "<b>4. </b>"
---

{{% notice info %}}
이번 Lab 은 FlexMatch 기능이나 FleetIQ 기능과 무관하게 GameLift 의 기능만을 순수하게 이용해서 오목 게임을 구현해봅니다. 일부 리전에서 아직 FlexMatch 기능이 제공되지 않는 경우, 본 실습 과정이 GameLift 시작에 유용하게 사용되어질 수 있습니다.
{{% /notice %}}

### 실습 목차 
**1. 게임 결과 데이터 처리를 위한 Lambda 함수 작성**    
**2. 랭킹 보드 구성을 위한 API Gateway 작성**    
**3. S3 기반의 랭킹 보드 웹 페이지 만들기**    
**4. 게임 서버 빌드 및 플릿 생성4**    
**5. 매치메이킹을 위한 EC2 인스턴스 생성**    
**6. 게임 클라이언트 구성하고 세션 게임에 연결**    


### Architecture Diagram

![Architecture](../images/gamelift/Architecture.png)

FlexMatch 기반의 아키텍처와 차이점은 **매치메이킹을 위한 별도의 인스턴스**를 구성한다는 점입니다. 이 부분에 중점을 두어 실습을 진행합니다. 나머지 과정에 큰 차이는 없습니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
