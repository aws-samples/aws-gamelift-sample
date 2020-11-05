---
title: GameLift FlexMatch 실습
url: /flexmatch
weight: 40
pre: "<b>2. </b>"
---


### GameLift 서비스와 함께 게임 서버 구축 <br/><br/>

{{% notice tip %}}
본 실습에서는 다양한 구현 아키텍처를 보여주기 위하여 FlexMatch 기능은 서버리스 매치메이킹을 이용합니다.   
이 내용은 GameLift Classic 실습 내용과 차이가 있습니다.
{{% /notice %}}

### Architecture Diagram (FlexMatch)

다음 아키텍처는 FlexMatch 실습의 아키텍처입니다.

![Architecture](../images/flexmatch/FlexDemo.png)

FlexMatch 의 내부 동작 로직은 기존 GameLift 의 사용과는 다릅니다. GameLift FlexMatch 기능을 사용하면 보다 다양한 기능을 높은 유연성을 가지고 설계할 수 있습니다. 다음 아키텍처는 본 실습 과정에서의 이해를 돕기 위해 FlexMatch 의 구성에 대해 설명합니다.

![Architecture](../images/flexmatch/GL-Flex.png)

기존에 별도의 매치메이커 서버를 두고, GameLift 가 관리하는 플릿의 인스턴스에 세션 게임 서버를 배치했었던 구조에서 서버리스 기반의 매치메이킹 구조로 바뀌었습니다.
이제, 매치 요청이 들어올 때에만 서버가 효율적으로 실행되고 매치메이킹 서버에 대한 관리 부담을 줄이면서 게임 세션을 매칭할 수 있습니다.


### 실습 목차

실습은 다음과 같은 순서로 진행됩니다. 

1. Lambda 함수 구성
2. API Gateway 구성
3. S3 웹 호스팅 환경 구성
4. 게임 서버 빌드 및 플릿 생성
5. 서버리스 기반의 FlexMatch 로직 및 Lambda API 구성하기
6. FlexMatch 게임 클라이언트와 환경을 연결 (게임 플레이!)
7. FlexMatch 매치메이킹 이벤트 활용
8. FlexMatch Ruleset 테스트



<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>


