---
title: ElastiCache 구성
url: /awsservice/lab12
weight: 30
pre: "<b>1-2. </b>"
---

### ElastiCache 구성하기 <br/><br/>

다음으로 ElastiCache를 설정합니다. 이것은 순위 정보를 저장할 것입니다.

1. AWS 콘솔에서 ElastiCache로 이동합니다. https://console.aws.amazon.com/elasticache
2. ElastiCache cluster를 생성합니다. 우리는 Redis 엔진을 사용할 것입니다. 왼쪽 메뉴에서 Redis 클러스터를 선택 후 Redis 클러스터 생성을 클릭 합니다.
3. 다음 스크립 캡처와 같이 필요한 정보를 입력해줍니다.

![Redis](../../images/awsservice/lab12/ElastiCache_Redis01.png)
![Redis](../../images/awsservice/lab12/ElastiCache_Redis02.png)
![Redis](../../images/awsservice/lab12/ElastiCache_Redis03.png)
![Redis](../../images/awsservice/lab12/ElastiCache_Redis04.png)

* 따로 설명이 되어있지 않은 설정은 모두 default로 진행합니다.

* Name: gomokuranking    
* Engine: 6.2 (5.0.4 이상의 버전을 권고드립니다)    
* Port: 6379 (default)    
* Parameter group: default    
* Number of replicas: 0    

4. 서브넷 그룹 설정에서는, 기존의 default vpc 및 subnet 을 선택하셔도 되고 실습을 위한 vpc 및 subnet group을 새로 생성하실 수도 있습니다.
5. 모든 설정이 완료되면, Create 버튼을 클릭하여 Redis 클러스터를 생성합니다. (시간이 조금 걸리기 때문에 다음 단계인 SQS 생성을 먼저 진행할 수도 있습니다. )
6. 생성한 Redis 클러스터의 상태가 available이 되면 Primary Endpoint를 따로 기록해둡니다. 추후 Lambda 생성 시에 해당 Endpoint가 사용됩니다.

![ElastiCache](../../images/awsservice/lab12/ElastiCache_DB.png)

7. 생성한 ElastiCache의 보안을 강화하기 위하여 gomokuranking 클러스터에 안전한 보안 그룹을 생성하여 할당합니다. 이번 실습에서는 아주 간단한 보안그룹 정책을 생성하여 할당하겠습니다.
8. VPC 콘솔에서 https://console.aws.amazon.com/vpc 좌측의 보안 그룹 메뉴를 선택하고 보안 그룹 생성 버튼을 클릭합니다.
9. Name tag, Group name 등에 적절한 정보를 입력해주고, VPC는 실습을 진행 중인 default VPC를 선택합니다. (리전에 VPC가 하나라면 default가 따로 표시되지는 않습니다)
10. 보안 그룹 내의 통신을 위하여 inbound 정책을 수정해야 합니다. 생성한 보안 그룹을 선택하고 인바운드 규칙 탭을 클릭합니다.
11. 인바운드 규칙 추가 버튼을 클릭하고 다음 스크린 캡처와 같이 정책을 생성합니다. 여기서 Source 에는 **보안 정책 자신의 Group ID**를 입력합니다. 이렇게 함으로써 이 보안 그룹을 할당한 호스트와 서비스들끼리 통신을 할 수 있습니다. 생성한 보안 그룹을 기억해 둡니다.

![SecurityGroup](../../images/awsservice/lab12/SecurityGroup.png)

* Type: All traffic    
* Protocol: All    
* Source: Security Group itself    

12. 보안 그룹 생성을 완료하였다면 다시 ElastiCache 페이지로 돌아와 생성한 Redis 클러스터를 선택합니다.
13. 클러스터를 선택하고 상단의 수정 버튼을 클릭하고, 수정 페이지의 보안 항목에서 방금 생성한 Security Group 을 선택한 뒤 반영합니다.

![ElastiCache_SG](../../images/awsservice/lab12/ElastiCache_SG.png)

ElastiCache 설정을 완료하였습니다. 다음으로 SQS 설정을 할 차례입니다.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
