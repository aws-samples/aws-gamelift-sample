---
title: Creating IAM Policy and Role
url: /awsservice/lab14
weight: 50
pre: "<b>1-4. </b>"
---

### Creating IAM Policy and Role <br/><br/>

As last of this part, we are creating IAM Policy and Role for Full Stack Application.

1. Firstly, making Policy for Role. In this lab, we are using AWS Managed Policy for comfort. Move to IAM menu on console. https://console.aws.amazon.com/iam

2. Select Policies on menu, and click Create Policy button.

3. Select GameLift on Visual Editor > Service. For Actions, we are selecting "All GameLift actions" for comfort. Click "Review Policy".

![IAM](../../images/awsservice/lab14/IAM_GameLift[en].png)

4. Put Policy Name(GameLiftFullAccess) and Click "Create policy" Button.

5. Nexly, we are creating five IAM Roles for Lambda functions.

6. Move IAM Console > Role menu, and click "Create Role" button.

7. Select AWS Lambda for Role Type.

![IAM_Lambda](../../images/awsservice/lab14/IAM_Lambda[en].png)

8. Allocate 3 IAM policies on this Role. Select Policies (AmazonSQSFullAccess, AmazonDynamoDBFullAccess, AWSLambdaBasicExecutionRole) on the editor.

9. Put Tag as an option, and Click Next:Review. Role Name would be "Gomok-game-sqs-process".

10. Click "Create role" button to create first IAM Role.

11. Creating second role is also similar. For this role, choose AmazonDynamoDBFullAcess, AmazonVPCFullAccess, AWSLambdaBasicExecutionRole as Permission.

12. Designate Role name as "Gomok-game-rank-update".

13. Third role is also similar. Let's set AmazonVPCFullAccess, AWSLambdaBasicExecutionRole policies for this Role.

14. Set role's name as "Gomok-game-rank-reader".

15. Creating fourth role uses Policy that we made above(GameLiftFullAccess). Let's put permissions AWSLambdaBasicExecutionRole, AmazonDynamoDBFullAccess, and GameLiftFullAccess for this Role.

16. Set its name as "Gomok-game-match-request".

17. Last Role will also be same. It should contain AWSLambdaBasicExecutionRole, GameLiftFullAccess Policies as permissions.

18. Name of last role would be "Gomok-game-match-status"

19. Creating all IAM Roles, you can find roles below!

![Gomok_Roles](../../images/awsservice/lab14/Gomok_roles.png)

20. Lastly, make additional Role for GameLift. Roles that we made before were used for Lambda functions, but this role is for GameLift Fleet. GameLift Fleet needs to access SQS for Game Result Processing.

21. Click "Create Role" and make new Role. We should select "GameLift" as Service, but for now select "EC2" as service, and move next.

22. Select AmazonSQSFullAccess as Permission, and set role name as "Gomoku-GameLiftFleetRole".

![GomokFleetRole](../../images/awsservice/lab14/GomokFleetRole[en].png)

23. We should change service that uses this Role to GameLift. Select "Gomoku-GameLiftFleetRole" on console, and select "Trust relationships" tab. Click "Edit trust relationship" button.

24. Change settings like below. It will be changed "ec2 -> gamelift". Save with "Update Trust Policy" button.

![GameLiftAssumeRole](../../images/awsservice/lab14/GameLiftAssumeRole.png)

This is all about first part on this lab. :)
No so complicated. right?

Let's make Game server on next part.

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
