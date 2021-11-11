---
title: FlexMatch 룰셋 테스트
url: /flexmatch/lab29
weight: 100
pre: "<b>2-9. </b>"
---

### FlexMatch 규칙세트 사용 <br/><br/>

이번 장에서는 몇가지 테스트해볼 수 있는 만들어진 규칙세트를 제공합니다. 뛰어난 매치메이킹은 유저들의 흥미를 돋구워주는데 충분하죠!    
오목이라는 1:1 게임의 특성 상 많은 규칙을 테스트해보기는 쉽지 않지만, 그래도 게임에 적용시켜볼 수 있을만한 규칙의 예제는 여러분의 게임에 영감을 줄 수 있습니다.    

규칙을 적용하기 위해서는 다음과 같이 매치메이킹 규칙 세트를 새로 만들어줍니다.

![Rule](../../images/flexmatch/lab29/Rule-1.png)

이후 이미 만들어진 규칙 구성 세트가 해당 규칙을 가리키도록 변경해줍니다. 이를 통해 쉽게 규칙 세트를 테스트해볼 수 있습니다.

![Rule](../../images/flexmatch/lab29/Rule-2.png)


* 기본 룰셋 : 기본 룰셋의 경우, 실습에 이미 적용되어 있습니다. 2-5 파트를 참고합니다.

* **점수에 엄격한 룰셋** : 일부 게임의 경우 점수 매칭이 엄격하게 적용되어질 수 있습니다. 가령, 승-패가 기록되고 명확히 우승을 가려야하는 토너먼트 방식의 게임이라면, 점수에 엄격하게 상대를 매칭시켜줘야 할 필요가 있습니다.

```json
{
    "name": "Strict Matching",
    "ruleLanguageVersion": "1.0",

    "playerAttributes" :
    [
        {
            "name" : "score",
            "type" : "number",
            "default" : 1000
        }
    ],
    "teams": [{
        "name": "player",
        "minPlayers": 1,
        "maxPlayers": 1,
        "quantity": 2
    }],

    "rules": [{
        "name": "SameScore",
        "description": "Only match players when they have same score",
        "type": "comparison",
        "operation": "=",
        "measurements": ["flatten(teams[*].players.attributes[score])"]
    }]
}
```

* **지연 시간을 제한하는 룰셋** : 스피드 매치가 중요한 게임의 경우 지연 시간을 중시한 Ruleset 을 테스트해볼 수 있습니다. 또한 글로벌 서버를 운영하는 경우에도, 지연 시간을 제한하면 유저들에게 쾌적한 매칭 경험을 제공해줄 수 있습니다.

```json
{
    "name": "Fast Game",
    "ruleLanguageVersion": "1.0",
    "playerAttributes" :
    [
        {
            "name" : "score",
            "type" : "number",
            "default" : 1000
        }
    ],
    "teams" :
    [
        {
            "name" : "blue",
            "maxPlayers" : 1,
            "minPlayers" : 1
        },
        {
            "name" : "red",
            "maxPlayers" : 1,
            "minPlayers" : 1
        }
    ],
    "rules": [{
        "name": "FastConnection",
        "description": "Prefer matches with fast player connections first",
        "type": "latency",
        "maxLatency": 50
    }],
    "expansions": [{
        "target": "rules[FastConnection].maxLatency",
        "steps": [{
            "waitTimeSeconds": 10,
            "value": 100
        }, {
            "waitTimeSeconds": 20,
            "value": 150
        }]
    }]
}
```

이 외에도 다양한 커스텀 룰셋을 구성하고 테스트할 수 있습니다. 보다 자세한 규칙 세트 작성 방법은 다음 링크를 참고합니다.
[https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/match-rulesets.html](https://docs.aws.amazon.com/ko_kr/gamelift/latest/developerguide/match-rulesets.html)


---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

