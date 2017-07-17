

## aws-gamelift-sample
This repository contains a sample game project using Amazon GameLift and AWS services including DynamoDB, Lambda, SQS, S3, EC2 and API-Gateway. This is a simple board game called [Gomoku](https://en.wikipedia.org/wiki/Gomoku), but technically and functionally identical to a real-time session-based online game. 

 - For Infra/System engineers, this project shows how to build a session-based online game directly on AWS. It also demonstrates the many benefits of AWS managed services such as GameLift.
 - For game developers, this project provides the architecture best practice and implementations of the most similar form to the actual session-based online game service. So they can leverage this asset to implement their own session-based online game directly on AWS.


### Sample Game Architecture
![Architecture Overview](web/gomoku_arch.png)


### Deployment
 - Play and Deployment guide [here](deployment/deployment.md)
 

### Future Work
 - Developing a Hands-on Technical Workshop to demonstrate the use of Amazon GameLift with AWS managed components for game services.
 - Developing a CloudFormation template for a one-click deployment
