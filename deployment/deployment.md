## Architecture Overview
![Architecture Overview](../web/gomoku_arch.png)


### DynamoDB Table -- Player Info
- Table Name: **GomokuPlayerInfo**
- HashKey: PlayerName (String)
- Attributes: Password (String), Score (Number), Win (Number), Lose (Number)
- Enable Stream with “New and Old images”
- Example Item

![Table item example](gomoku-ddb.png)


### SQS -- Game Result Queue
- Name: **game-result-queue**
- Queue configuration
![SQS Configuration](gomoku-sqs.png)
- Copy & Paste SQS Endpoint to Notepad. It will be used later in the Lambda function (Game Result Processing).


### ElastiCache -- Redis Sorted Set for Ranking 
- Select Redis 3.2.x
- Mostly set to default
- Set the VPC and Security Group: only traffic allowed from this Security Groups
- Copy & Paste Cluster Endpoint to Notepad when the Status is available. It will be used later in the Lambda function (Scoring and GetRanking).


### Lambda -- Game Result Processing
- Name: **game-sqs-process**
- Runtime: Python 2.7
- Role: grant Full Access to SQS and DynamoDB
- Advanced Settings
  - Memory: 128
  - Timeout: 1 min
- Triggers: CloudWatch Events rate 1 minute
- Edit code [GameResultProcessing.py](../Lambda/GameResultProcessing.py) and upload it as a deployment package
  -	Replace *region_name* and *queue_url* in the code with your AWS region and SQS endpoint url.
  -	For your information: [here](http://docs.aws.amazon.com/lambda/latest/dg/lambda-python-how-to-create-deployment-package.html) is how to create a deployment package for Lambda (python). 


### Lambda -- Scoring (DynamoDB streams to Redis)
- Name: **game-rank-update**
- Runtime: Python 2.7
- Role: grant Full Access to DynamoDB, VPC, and DynamoDB Streams
- Advanced Settings
  - Memory: 192
  - Timeout: 1 min
  - VPC: same with ElastiCache’s
  - Security Group: same with ElastiCache’s
- Triggers: DynamoDB Streams from *GomokuPlayerInfo*
- Edit code [Scoring.py](../Lambda/Scoring.py) and upload it
  - Replace Redis *host* in the code with your ElastiCache cluster endpoint.


### Lambda -- GetRanking
- Name: **game-rank-reader**
- Runtime: Python 2.7
- Role: grant Full Access to VPC
- Advanced Settings
  - Memory: 128
  - Timeout: 1 min
  - VPC: same with ElastiCache’s
  - Security Group: same with ElastiCache’s
- Edit code [GetRank.py](../Lambda/GetRank.py) and upload it
  - Replace Redis *host* in the code with your ElastiCache cluster endpoint.


### API Gateway -- Get Ranking Board
- Create API Name: **gomokuranking**
- Create Method: GET -- Lambda Integration -- Select region you created a Lambda, *game-rank-reader* -- Select the Lambda and Save.
- Deploy API: give a stage name and remember the Invoke URL. It will be used later in the S3 static website [main.js](../web/main.js).
- API Example
![API for Get Ranking](gomoku-apigw.png)


### S3 -- Static Web Page for Ranking Board
- Create a bucket for static website hosting and enable Bucket hosting.
- Edit code [main.js](../web/main.js) and then replace *http endpoint in $.get()* fucntion with your API Gateway Invoke URL.
- Upload files in *web* folder to S3 with public read accessible.
- You can access the ranking board page by S3 static website enpoint.


### GameLift -- Game Server Build & Fleet Creation
- Create an IAM User who can access SQS. (Game Result Queue)
  - Get ACCESS_KEY and SECRET_KEY (this will be used below config.ini)
- Open GomokuServer project in your Visual Studio 2015.
- Build GomokuServer with x64 release mode.
- Put the following files in one folder (a.k.a. server build folder)
  - GomokuServer.exe, aws-cpp-sdk-*.dll (from GomokuServer build folder)
  - config.ini and install.bat (from GomokuServer project root folder)
  - aws-cpp-sdk-gamelift-server.dll (from GomokuServer lib folder)
  - vc_redist_x64.exe (from [here](https://www.microsoft.com/en-us/download/details.aspx?id=48145))
- Edit config.ini file in the server build folder and fill in the required information like the following example

<pre>
    [config]
    SQS_REGION = ap-northeast-1
    SQS_ACCESSKEY = AKIAIRMOWERYX33....
    SQS_SECRETKEY = lBp+6Dg/wHGwo2I342......
    SQS_ENDPOINT = https://sqs.ap-northeast-1.amazonaws.com/....
</pre>	

- In the server build folder, upload the game server build by using AWS CLI.
![Game Server Upload](gomoku-server-upload.png)
- Check the Build in AWS GameLift web console.
![GameLift Build](gomoku-gamelift-build.png)
- Select the Build and then click “Create fleet from build”
  - Name: GomokuGameServerFleet-1
  - Instance type: c3.large or c4.large
  - Launch path: GomokuServer.exe
  - Concurrent processes: 50
  - EC2 Port settings
     - Port range: 49152-60000
     - Protocol: TCP
     - IP address range: 0.0.0.0/0
  - Initialize fleet! and wait until the fleet is ACTIVE
     - (cf.) New -- Downloading -- Validating -- Activating -- Active
  - When the fleet is ACTIVE, you can see the web console like this.
![GameLift Fleet](gomoku-gamelift-fleet.png)
- Then, move to Aliases on the GameLift Menu and click “Create alias”
- Put any alias name and description
- Associated fleet with you had created above. (GomokuGameServerFleet-1)
- Click “Configure alias”
- You can see the created Alias for the Fleet like this
![GameLift Alias](gomoku-gamelift-alias.png)
- Then you should copy & paste the Alias ID to the notepad because we will use the Alias ID as a Fleet endpoint.


### EC2 -- MatchMaker Server
- Open GomokuMatchMaker project in your Visual Studio 2015.
- Build GomokuMatchMaker with x64 release mode
- Put the following files in one folder (a.k.a. MatchMaker folder)
  - GomokuMatchMaker.exe, aws-cpp-sdk-*.dll (from the build folder)
  - config.ini (from GomokuMatchMaker project root folder)
- Edit config.ini file in the MatchMaker folder and fill in the required information like the following example

<pre>
    [config]
    GAMELIFT_ALIAS = alias-291ce0bf-938a-4800-b0d5-....
    LISTEN_PORT = 5999
    GAMELIFT_REGION	= ap-northeast-1
    PLAYER_TABLENAME = GomokuPlayerInfo
</pre>

- Create an EC2 IAM role which has Full Access about DynamoDB and GameLift.
- Launch and Instance:  Microsoft Windows Server 2016 Base
  - Instance type: t2.large or above
  - Change the Security Group setting to allow inbound connection like this:
     - Port: 5999, Protocol: TCP, Source: 0.0.0.0/0
  - Associate the EC2 IAM role with created above.
- When the EC2 instance up, connect the EC2 instance by Windows Remote Desktop.
- Add a Rule for Windows Firewall Settings in Control Panel which allow TCP port listening to 5999.
- Copy the MatchMaker folder to the EC2 instance.
  - Execute vc_redist_x64.exe for the first time. (just first time)
  - Run GomokuMatchMaker.exe
- Create Elastic IP (EIP) and associate with the MatchMaker EC2 instance.
  - Take a note the EIP Address. (This will be used at GomokuClient.)

### Game Client Configuration and Play!
- Open GomokuClient project in your Visual Studio 2015.
- Build GomokuClient with x64 release mode.
- Put the following files in one folder. (Client folder)
  - GomokuClient.exe, freeglut.dll, glew32.dll, glfw3.dll (from the build folder)
  - config.ini (from GomokuClient project root folder)
- Edit config.ini file and fill in the required information like the following example.
   - Set the SERVER_IP with the EIP address allocated above step.
   - Set the player name and password.

<pre>
    [config]
    SERVER_IP = 52.197.xxx.xxx
    PORT_NUM = 5999
    PLAYER_NAME = Amazonian
    PLAYER_PASSWD = simplepw00
</pre>

  - Note about *PLAYER_NAME* and *PLAYER_PASSWD*
    - At the first access, the match maker server will create an account on the DynamoDB with the corresponding PLAYER_NAME and PLAYER_PASSWD.
    - If the name for PLAYER_NAME already exists on the DynamoDB, the match maker compares PLAYER_PASSWD with the password stored on the DynamoDB.
    - If the passwords match, the authentication is successful and you can continue playing the game.
- Execute GomokuClient.exe
- Click the mouse right button, and Start!
- Please wait until the matchmaking is completed with other players. Enjoy!
