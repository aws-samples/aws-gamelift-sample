---
title: 실습 사전 준비
url: /intro/lab02
weight: 25
pre: "<b>0-2. </b>"
---

### 실습 소스 및 바이너리 파일 다운로드

* 실습에 사용되는 소스 파일은 다음 위치에서 다운로드할 수 있습니다.    
[https://github.com/aws-samples/aws-gamelift-sample](https://github.com/aws-samples/aws-gamelift-sample)

* 실습에서는 빌드 과정의 부담을 덜기 위해 사전 빌드된 바이너리를 제공합니다. 각 과정에서 필요한 바이너리 파일들은 위에서 받을 수 있는 루트 폴더의 bin 폴더 하위에 존재합니다. 다음 링크에서 완성된 바이너리 파일의 형태를 다운받으실 수도 있습니다.
[http://d2511y3q5icxx2.cloudfront.net/DemoBinary.zip](http://d2511y3q5icxx2.cloudfront.net/DemoBinary.zip)

보다 자세한 사항은 각 실습 항목에서 설명을 참고합니다.


### AWS CLI 설치

* 본 실습을 위해서는 최신 버전의 CLI 사용이 권고됩니다. 만약 AWS CLI 가 설치되어 있지 않다면 다음 링크를 참고해서 설치하실 수 있습니다.    
[https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv1.html](https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv1.html)

### AWS CLI v2 설치 (옵션)

* FleetIQ 실습을 위해서는 CLI v2 가 필요합니다. 다음 링크로부터 CLI 커맨드를 실행할 환경에서 CLI 버전 2를 설치하도록 합니다.    
Windows : [https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv2-windows.html](https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv2-windows.html)    
Mac OS : [https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv2-mac.html](https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv2-mac.html)    
Linux : [https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv2-linux.html](https://docs.aws.amazon.com/ko_kr/cli/latest/userguide/install-cliv2-linux.html)


### Windows 환경 구성

* GameLift 및 FlexMatch 실습은 Visual Studio 2015 에서 개발된 바이너리에 기반합니다.    
윈도우 환경 구성을 위해 서버 및 클라이언트 환경에 다음 C++ 재배포 가능 패키지가 요구됩니다.
[https://www.microsoft.com/en-us/download/details.aspx?id=48145](https://www.microsoft.com/en-us/download/details.aspx?id=48145)

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
