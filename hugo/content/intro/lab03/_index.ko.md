---
title: 실습 사전 지식
url: /intro/lab03
weight: 35
pre: "<b>0-3. </b>"
---

{{% notice info %}}
본 실습은 풀스택 게임 서비스를 만들기 위해 다양한 AWS 서비스들을 이용합니다.    
이번 장에서는 실습에서 사용되는 몇가지 서비스들을 간략하게 가이드합니다.
{{% /notice %}}

### GameLift
* Amazon GameLift 는 세션 기반 멀티플레이어 게임을 위해 제공되는 관리형 AWS 서비스입니다.
* GameLift 는 AWS 글로벌 컴퓨팅 인프라 기반위에서 고성능과 높은 안정성을 제공하며, 비용 효율적으로 리소스를 사용할 수 있도록 지원합니다.
* 현재 GameLift 는 세션 기반 게임을 위한 관리형 호스팅, Realtime 서버 구축을 위한 관리형 호스팅, 그리고 EC2 에서의 직접 게임 호스팅을 위한 독립형 FleetIQ 기능을 제공합니다.
[https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/gamelift-intro.html](https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/gamelift-intro.html)


### DynamoDB
* Amazon DynamoDB 는 AWS 에서 제공하는 관리형 NoSQL 데이터베이스로, 뛰어난 확장성과 내구성, 고성능을 제공하는 분산 데이터베이스입니다.
* Database Connection 관리 및 분산 환경에서 뛰어난 확장성으로 인해 Lambda 기반의 서버리스 아키텍처에서도 우수하게 사용될 수 있는 데이터베이스입니다.
* 높은 수준의 트래픽을 처리할 수 있으며 클라우드 기반 분산 환경에 적합한 데이터베이스로, 본 실습에서는 유저 정보 저장을 위해 DynamoDB 를 사용합니다.
[https://docs.aws.amazon.com/ko_kr/amazondynamodb/latest/developerguide/Introduction.html](https://docs.aws.amazon.com/ko_kr/amazondynamodb/latest/developerguide/Introduction.html)


### ElastiCache
* Amazon ElastiCache 는 Redis 및 Memcached 와 호환되는 완전관리형 인메모리 데이터베이스입니다.
* 고성능의 캐시를 제공하는 인메모리 데이터베이스로, 이번 실습에서는 랭킹정보 관리를 위한 캐시로 사용됩니다.
[https://docs.aws.amazon.com/ko_kr/AmazonElastiCache/latest/red-ug/WhatIs.html](https://docs.aws.amazon.com/ko_kr/AmazonElastiCache/latest/red-ug/WhatIs.html)


### SQS
* Amazon SQS 는 뛰어난 내구성을 제공하는 대기열을 제공합니다. 이를 통해 분산 시스템 아키텍처에서 컴포넌트들을 디커플링하여 보다 유연한 아키텍처를 구성하실 수 있습니다.
* 이번 실습에서는 게임 결과를 SQS 를 통해 처리하는 식으로 구현합니다.
[https://docs.aws.amazon.com/ko_kr/AWSSimpleQueueService/latest/SQSDeveloperGuide/welcome.html](https://docs.aws.amazon.com/ko_kr/AWSSimpleQueueService/latest/SQSDeveloperGuide/welcome.html)


### Lambda
* AWS Lambda 는 서버를 프로비저닝하거나 관리하지 않고도 코드를 실행할 수 있도록 제공해주는 컴퓨팅 서비스입니다.
* 별도의 컴퓨팅 인프라 구성이 필요없이 필요할 때에만 쉽게 서비스를 시작할 수 있는 서버리스 아키텍처의 핵심 서비스입니다.
* 이번 실습에서는 각종 API 처리를 Lambda 로 구성합니다.
[https://docs.aws.amazon.com/ko_kr/lambda/latest/dg/welcome.html](https://docs.aws.amazon.com/ko_kr/lambda/latest/dg/welcome.html)


### API Gateway
* Amazon API Gateway 는 규모에 관계없이 REST API 및 WebSocket API 를 제공할 수 있도록 지원하는 서비스입니다.
* 쉽고 다양한 기능을 기반으로 API 구성을 시작할 수 있습니다.
* 이번 실습에서는 Lambda 를 API Gateway 를 이용해서 트리거합니다.
[https://docs.aws.amazon.com/ko_kr/apigateway/latest/developerguide/welcome.html](https://docs.aws.amazon.com/ko_kr/apigateway/latest/developerguide/welcome.html)


### S3
* Amazon S3 는 인터넷으로 제공되는 오브젝트 스토리지 서비스입니다.
* S3 를 통해 쉽게 웹 기반 워크로드에서 스토리지를 사용하고 웹서비스를 호스팅할 수 있습니다.
* 이번 실습에서는 S3 를 이용해서 유저들의 정보를 조회하는 웹 호스팅을 시작합니다.
[https://docs.aws.amazon.com/ko_kr/AmazonS3/latest/dev/Welcome.html](https://docs.aws.amazon.com/ko_kr/AmazonS3/latest/dev/Welcome.html)

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
