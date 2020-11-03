---
title: FleetIQ 맛보기
url: /fleetiq
weight: 50
pre: "<b>3. </b>"
---

### GameLift 독립형 FleetIQ 기능 사용하기

앞서 실습에서 우리는 GameLift 의 스팟 플릿 구성을 통해 세션 게임 서버들을 배치할 수 있었습니다. 이 기능을 통해 온디맨드 대비 60-80% 의 비용 절감 효과를 얻을 수 있었고, 이는 GameLift 내부에서 FleetIQ 라는 기능을 통해 스팟 인스턴스의 중단률을 낮추어 운용률을 높여주었습니다.    
FleetIQ 기능이 이제는 독립형으로 제공됩니다. FleetIQ 기능을 통해 낮은 비용의 스팟 인스턴스를 보다 안전하게 사용하면서도 EC2 리소스 및 Auto Scaling Group 을 직접 관리하고, 선택적으로 다른 AWS 와 유연하게 통합할 수 있습니다. 즉, 보다 저렴한 가격으로 개발 및 운영에 있어서 높은 유연성을 가질 수 있습니다!

{{% notice info %}}
그럼 이번 실습을 통해 어떻게 저렴한 컴퓨팅 비용으로 게임 서버를 구성할 수 있는지 간단하게 알아보도록 하겠습니다.
독립 실행형 FleetIQ 는 현재 Preview 로 제공되는 기능이므로, 본 자료는 지속적으로 업데이트될 예정입니다.
{{% /notice %}}

### Architecture Diagram

FleetIQ 기능은 GameLift 의 기능으로 포함되어 있지만, 독립적인 아키텍처를 구성합니다.

![FleetIQ](../images/fleetiq/FleetIQ.png)

FleetIQ 를 이용하는 서버와 클라이언트는 FleetIQ 가 지원되는 SDK 나 CLI, 또는 API 를 이용해서 구현될 수 있습니다.
이는 GameLift 와 독립적인 기능을 이용하며, GameLift 에 종속되지 않습니다.
실습을 통해 어떻게 이용할 수 있는지 알아보겠습니다.

보다 자세한 FleetIQ 의 구조는 다음 링크를 참고해보실 수 있습니다.

[https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/gsg-intro.html](https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/gsg-intro.html)


---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
