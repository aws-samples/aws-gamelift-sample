---
title: Deploy CloudFormation Stack
url: /awsservice/lab10
weight: 10
pre: "<b>1-0. </b>"
---

### Automate AWS resources with CloudFormation <br/><br/>

In this page, it is able to create AWS services for this lab with CloudFormation Stack.
It is useful to prevent errors or misconfigurations by this Automated Stack, and also proper to start GameLift HoL quickly.
If you finish CloudFormation Stack deployment, you are able to proceed to [next part](../../flexmatch).

1. Access to CloudFormation console of your region. (https://ap-northeast-2.console.aws.amazon.com/cloudformation)

2. Find "GomokuHoLTemplate.json" file on your source files. This file is included in Folder "CloudFormation".

3. Click "Create Stack" on CloudFormation Console. And select **Template is ready > Upload a template File**. Upload GomokuHoLTemplate.json.   

![EFN](../../images/awsservice/lab10/EFN-1.png)

4. If you check "View in Designer", you can check CloudFormation Stack like below. It is easy to configure automated stacks using CloudFormation.

![EFN](../../images/awsservice/lab10/EFN-0.png)


5. Input Stack name "GameLiftHoL".

6. Create stack as follows. Please check resource is deployed successfully like below.    

![EFN](../../images/awsservice/lab10/EFN-3.png)

* If resource with same name already exists in same region, it is not able to make resource. If same resources are remained, retry deployment after deleting remained resources.   

After creating AWS services with CloudFormation Stacks, you can skip lab-1. Move on to the [next part](../../flexmatch)

---
<p align="center">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
