---
title: 부록(Appendix)
url: /appendix
weight: 80
pre: "<b>6. </b>"
---

### GameLift 바이너리 빌드

{{% notice info %}}
GameLift 의 게임 서버 및 클라이언트 바이너리는 모두 Visual Studio 2015 기반에서 제작되었습니다. 해당 버전과 호환되는 Visual Studio 환경에서 빌드하시기를 권고드립니다.
{{% /notice %}}

1. 제공된 소스 코드에서 GomokuClient 또는 GomokuServer 폴더 중 빌드하고자 하는 프로젝트를 선택합니다.

2. 프로젝트를 Visual Studio 환경에서 오픈합니다. 프로젝트는 Nuget 패키지 기반 Dependency 를 갖고 있으며, AWS SDK 들에 대해 DLL Dependency 를 갖고 있습니다.

3. 다음과 같이 프로젝트 세팅을 구성하고 빌드합니다.

![APP](../images/appendix/APP-1.png)

빌드 환경을 Release 로 구성하시는 것을 권고드립니다. 기본 폴더에 관련 Dependency 를 이미 포함하고 있습니다. 만약 문제가 생긴다면 Release 폴더의 dll 파일 등을 복사하여 debug 폴더에도 동일한 환경을 구성 후 테스트해보실 수 있습니다.

**GameLift Classic 프로젝트와 GameLift FlexMatch 프로젝트는 서로 다른 버전의 SDK 를 사용하므로 구현 상에 차이가 있습니다. 각 환경에 맞게 프로젝트를 빌드하시는 것을 권고드립니다.**


### GameLift 게임 서버 접속

GameLift 에서 게임 서버 리소스를 관리하지만, 디버깅이나 로그 분석 등의 이유로 게임 서버에 직접 접속해야할 수 있습니다. 이럴 때 다음 지침을 참고합니다.

아래 커맨드 사용을 위해서는 CLI 가 필요합니다.

1. FleetID 로부터 해당 플릿에 구성된 인스턴스 목록을 조회합니다.

```sh
aws gamelift describe-instances --fleet-id fleet-4b7fc542-cb08-4f65-908c-737f64ca2e8f --region ap-northeast-2
```

2. 위의 커맨드로 알게된 Fleet ID 와 인스턴스 ID 를 기반으로 다음 명령어를 수행합니다.
```sh
aws gamelift get-instance-access --fleet-id fleet-4b7fc542-cb08-4f65-908c-737f64ca2e8f --instance-id i-031278fb03cf29d35 --region ap-northeast-2
```

3. 도출된 인스턴스 접속 정보를 이용해 RDP 또는 SSH 로 인스턴스에 접근합니다. 이 때 인스턴스의 포트 정보가 접근 가능한지 확인합니다. 만약 포트가 열려있지 않다면 플릿을 편집하여 포트 구성을 수정할 수 있습니다.

![APP](../images/appendix/APP-2.png)

위의 그림에서 인스턴스는 RDP 포트에 대해 허용하고 있는 모습입니다. 환경에 맞게 구성합니다.

---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
