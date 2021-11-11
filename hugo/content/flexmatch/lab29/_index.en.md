---
title: Test FlexMatch Ruleset
url: /flexmatch/lab29
weight: 100
pre: "<b>2-9. </b>"
---

### Test FlexMatch Ruleset <br/><br/>

There are some rulesets for you to test. Excellent Matchmaking enough to facinate users very well!
We provide some examples for you, and we hope these rulesets can help you to inspire making your ruleset.

In this page, we are testing how can apply new rule to your game easily.

Let's make Matchmaking Ruleset for new Ruleset.

![Rule](../../images/flexmatch/lab28/Rule-1[en].png)

You can test rulesets below. 

And move to Matchmaking configurations. There would be configuration that made previously.
You can edit matchmaking configuration like below. You can change ruleset that you made above.

![Rule](../../images/flexmatch/lab28/Rule-2[en].png)


* Basic Ruleset : For basic ruleset, it has already applied on current game. Please refer page 2-5.

* **Strict Ruleset** : Some games need strict types of Matchmaking. For example, games like tournament require ratings evaluated by Win or Lose should match opponents strictly.

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

* **Fast Match Ruleset** : There are some types of games which is very important for players to match other players. If you would like to consider making global games, it can also be important. Most of users want to play games quickly with low latency.

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

{{% notice tip %}}
Except for these examples above, you can make your own ruleset for the games. Please refer below link, and test it.
[https://docs.aws.amazon.com/gamelift/latest/developerguide/match-rulesets.html](https://docs.aws.amazon.com/gamelift/latest/developerguide/match-rulesets.html)
{{% /notice %}}

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>

