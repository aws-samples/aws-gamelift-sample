---
title: CloudFormation 스택 생성
url: /awsservice/lab10
weight: 10
pre: "<b>1-0. </b>"
---

### CloudFormation 을 통한 스택 생성 자동화 <br/><br/>

이번 장에서는 앞선 AWS 서비스 구성 단계들을 CloudFormation 을 이용해서 간소화된 스택으로 구성합니다.    
이번 단계는 직접 서비스들을 구성하는 단계에서의 실수를 줄이고, 보다 빠르게 GameLift 를 이용한 실습을 하고자할 경우에 적합합니다.    
이번 단계에서 서비스들을 배포하셨다면, 1장의 나머지 단계는 건너뛰셔도 좋습니다.    

1. 실습 환경을 갖추고자 하는 리전의 CloudFormation 콘솔로 접속합니다. (https://ap-northeast-2.console.aws.amazon.com/cloudformation)

2. 다운로드받은 소스 파일에서 CloudFormation 폴더에 "GomokuHoLTemplate.json" 항목을 찾습니다.

3. CloudFormation 콘솔에서 "스택 생성 항목을 누른 뒤, 템플릿 지정 > 템플릿 파일 업로드를 선택합니다. GomokuHoLTemplate.json 파일을 업로드합니다.

![CFN](../../images/awsservice/lab10/CFN-1.png)

4. 디자이너로 보면 다음과 같은 CloudFormation 스택을 확인하실 수 있습니다. 다양한 리소스 스택에 대해 다음과 같이 자동화된 스택 구성이 가능합니다.

![CFN](../../images/awsservice/lab10/CFN-0.png)
s
5. 스택 이름을 GameLiftHoL 로 입력합니다.

6. 절차에 따라 스택을 생성합니다. 다음과 같이 모든 리소스가 정상적으로 배포되는지 확인합니다.

![CFN](../../images/awsservice/lab10/CFN-3.png)

* 동일한 리소스가 이미 리전에 존재하는 경우 리소스가 만들어지지 않을 수 있습니다. 만약, 동일한 리소스가 존재한다면 만들어진 리소스들을 지우고 다시 시도합니다.

이 장의 실습을 마치셨다면 [다음 장 실습](../../flexmatch)으로 이동합니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
