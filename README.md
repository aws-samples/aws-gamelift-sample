

# aws-gamelift-sample (with FlexMatch)
This repository contains a sample game project using Amazon GameLift and AWS serverless services including DynamoDB, Lambda, SQS, S3, and so on. This is a simple board game called [Gomoku](https://en.wikipedia.org/wiki/Gomoku), but technically and functionally identical to a real-time session-based online game. 

<img src="web/gomoku.png" width="600" height="600"/>

 - For Infra/System engineers, this project shows how to build a session-based online game directly on AWS. It also demonstrates the benefits of AWS managed services such as GameLift.
 - For game developers, this project provides architecture best practice and implementations of the most similar form to the actual session-based online game service. So they can leverage this asset to implement their own session-based online game on AWS.

# CloudFormation
This repository includes CloudFormation for deployment automation. You can start it easily with guide below.ß

# FleetIQ sample
This repository contains Python based simple TCP sample using FleetIQ.
Since FleetIQ is on Preview stage, this repository will be updated continously.


## Sample Game Architecture (Serverless with FlexMatch)
![Architecture Overview](web/gomoku_arch.png)

## Deployment
 - Play and Deployment guide 
     - Korean version [here](http://gamelift-workshop.s3-website.ap-northeast-2.amazonaws.com/ko/)
     - English version [here](http://gamelift-workshop.s3-website.ap-northeast-2.amazonaws.com/en/)
 - Game Client Run guide
     - MS C++ Redistributable package (https://www.microsoft.com/ko-kr/download/details.aspx?id=48145)

## Future Work
 - Developing a Hands-on Technical Workshop to demonstrate the use of Amazon GameLift with AWS managed components for game services.
 - Developing Demo environment based on Cloud9 IDE.
 - Update FleetIQ demo continously.
 - Developing multi-region game support feature.

## For further questions/ assistances
 - Please contact korea-gaming-sa@amazon.com for any comment/ requests regarding this content 
