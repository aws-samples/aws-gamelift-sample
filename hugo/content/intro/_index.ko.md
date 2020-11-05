---
title: GameLift HoL 소개
url: /intro
pre: "<b>0. </b>"
weight: 20
chapter: true
---

# GameLift 로 시작하는 Full Stack Game

---

지난 수개월간 만들어온 여러분의 게임이 드디어 완성을 앞두고 있습니다.
"오목" 은 가로 세로 19칸으로 그어진 선 위에서 우주만물의 음과 양을 상징하는 검은 돌과 흰 돌을 사용하는 게임입니다.
두 명의 플레이어는 그들의 검은 돌과 흰 돌을 각각 격자에 놓게 되고, 처음으로 5개의 돌을 직선(혹은 대각선)으로 만든 플레이어가 승리하게 됩니다.

하지만 게임은 만들었는데, 생각치 못한 점을 발견하게 됩니다. 이럴수가... 오목은 혼자 할 수는 없다는 것이었죠!
여러분은 몇가지 AWS 서비스를 통해 "인터넷" 을 통한 플레이를 제공하기로 설정했습니다.
힘들 것 같나요? 하지만 이번 데모에서 게임은 이미 준비되어 있습니다.

한가지만 명심하면 됩니다. 여러분에게는 큰 운영(Ops) 조직이 없기 때문에 Serverless 및 관리형 서비스를 활용하여 최대한 효율적이고 최적화된 방식을 활용해서 만들어야 합니다.
즉, API Gateway, Lambda, ElastiCache(Redis), S3, DynamoDB, SQS 그리고 GameLift 를 사용해서 Full Stack 을 만들어보겠습니다!

---

## Today's Demo

![Game](../images/intro/Game.png)

오늘의 목표가 되는 게임입니다. 정말 재미있어 보이는군요! 재미있는 게임 만들기를 위해 하나하나 차근차근 따라가면서 실습을 진행하시는 것을 권장드립니다.


<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
