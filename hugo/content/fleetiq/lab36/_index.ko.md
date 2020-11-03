---
title: 서버 - 클라이언트 연동
url: /fleetiq/lab36
weight: 90
pre: "<b>3-6. </b>"
---

### FleetIQ 기반의 서버 - 클라이언트 연동
1. 서버 인스턴스에 연결된 Cloud9 IDE 로 이동합니다.

2. 다음 그림과 같이 Python 3 로 서버 스크립트를 작동시킵니다.

![IQ](../../images/fleetiq/lab36/IQ-1.png)

3. 서버가 실행되면 클라이언트의 연결을 대기하게 됩니다. 다음으로 클라이언트 실행을 위해 클라이언트 Cloud9 IDE 로 이동합니다.

4. 같은 방법으로 클라이언트 스크립트를 실행시켜 줍니다. 다음과 같이 에코 클라이언트가 호출되게 됩니다!    이제 서버와 TCP 소켓 통신을 확인해보세요.

![IQ](../../images/fleetiq/lab36/IQ-2.png)

5. 주목할 수 있는 점은 FleetIQ 서비스로부터 연결 정보를 받아서 Fiesta 알고리즘에 기반한 효율적인 인스턴스로 세션 연결을 구성했다는 점입니다. 이제 독립형 FleetIQ 기능을 통해 세션 기반 게임 서비스를 유연하고 비용 효율적으로 구축하실 수 있습니다.


---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
