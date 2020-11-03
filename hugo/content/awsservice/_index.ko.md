---
title: 풀스택 AWS 서비스 구성
url: /awsservice
weight: 30
pre: "<b>1. </b>"
---

{{% notice info %}}
이번 Lab에서는 본격적으로 게임을 만들기 앞서, 멀티플레이 게임을 더 풍부하게 만들 수 있는 사용자 데이터베이스와 게임 결과를 처리하는 큐, 및 각종 AWS 리소스 접근을 위한 권한 등을 생성해볼 것입니다.
{{% /notice %}}

### CloudFormation 으로 실습 단계 간소화

{{% notice tip %}}
이 장의 단계는 CloudFormation 을 이용한 자동화된 스택으로 간소화할 수 있습니다. 보다 빠른 진행을 원하신다면, 다음 링크를 참조해주세요.
{{% /notice %}}


* [CloudFormation 을 통해 빠른 실습 진행](./lab10)

위의 단계 진행 이후에는 바로 2장으로 넘어가실 수 있습니다.

### 직접 AWS 서비스 스택 구성

{{% notice tip %}}
직접 풀스택 게임 구성을 위한 AWS 서비스들을 생성하실 수 있습니다. 1-1 부터의 단계에서 시작합니다.
{{% /notice %}}

1. [사용자 정보 저장을 위한 DynamoDB 만들기](./lab11)
2. [리더 보드 제작을 위한 ElastiCache 구성](./lab12)
3. [게임 결과 처리를 위한 SQS 만들기](./lab13)
4. [AWS 리소스 접근을 위한 IAM 정책 및 역할 생성](./lab14)


---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
