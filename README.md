

# aws-gamelift-sample (with FlexMatch)
This repository contains a sample game project using Amazon GameLift and AWS serverless services including DynamoDB, Lambda, SQS, S3, and so on. This is a simple board game called [Gomoku](https://en.wikipedia.org/wiki/Gomoku), but technically and functionally identical to a real-time session-based online game. 

<img src="web/gomoku.png" width="600" height="600"/>

 - For Infra/System engineers, this project shows how to build a session-based online game directly on AWS. It also demonstrates the benefits of AWS managed services such as GameLift.
 - For game developers, this project provides architecture best practice and implementations of the most similar form to the actual session-based online game service. So they can leverage this asset to implement their own session-based online game on AWS.


## Sample Game Architecture (Serverless with FlexMatch)
![Architecture Overview](web/gomoku_arch.png)


## Deployment Guide
 - Play and Deployment guide [here](deployment/deployment.md)
 
