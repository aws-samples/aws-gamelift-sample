---
title: FleetIQ 기반 서버 생성
url: /fleetiq/lab34
weight: 70
pre: "<b>3-4. </b>"
---

### FleetIQ 서버 개발 환경 구축    

FleetIQ 서버 - 클라이언트 기능은 GameLift 에 종속되지않는 독립형 기능입니다.
현재 CLI 및 SDK 형태로 구현을 제공하고 있고, 본 실습에서는 Python 기반의 TCP 통신을 구현해보도록 하겠습니다.    
간단하면서도 직접 FleetIQ 의 동작 원리를 이해하고 서버 - 클라이언트 구조를 생성해볼 수 있는 좋은 예제입니다.    

1. Cloud9 콘솔로 접속합니다. (https://console.aws.amazon.com/cloud9/home)

2. 다음과 같이 새 IDE 를 만듭니다. 이 IDE 를 통해 원격 게임 서버의 코드를 편집할 것 입니다.

![IQ](../../images/fleetiq/lab34/IQServer-1.png)

3. 연결 구성을 SSH 로 설정하고, 방금 전에 만든 게임 서버 그룹의 게임 서버 인스턴스의 엔드포인트를 연결합니다. 이 때, 아래쪽의 SSH 키를 복사해서 연결해주어야 합니다.

![IQ](../../images/fleetiq/lab34/IQServer-2.png)

4. SSH 키를 카피 하셨다면, 다시 EC2 콘솔로 이동하고, 다음과 같이 해당 인스턴스에 접속합니다.

![IQ](../../images/fleetiq/lab34/IQServer-20.png)

접속에 대한 상세한 가이드는 다음 [링크](https://docs.aws.amazon.com/ko_kr/AWSEC2/latest/UserGuide/Connect-using-EC2-Instance-Connect.html)를 참고합니다.

```ssh
vi ~/.ssh/authorized_keys
```

커맨드를 입력하고, 나오는 인증키 정보 창에서 맨 뒤에 복사한 값을 붙여넣기 합니다.

```ssh
ssh-rsa ~~~ key
ssh-rsa @@@ xxx@amazon.com
```

과 같은 형태로 텍스트 파일이 구성되게 됩니다.

**쉽게 구성하시기 위해서는 vi 기준으로 A 를 입력하시면 파일의 맨 끝으로 이동합니다. 엔터키를 치신 후 붙여넣기하고 저장합니다.**

5. 서버 인스턴스의 콘솔에서 다음 스크립트를 입력합니다.

```ssh
curl -L https://raw.githubusercontent.com/c9/install/master/install.sh | bash
```

위의 스크립트를 통해 간편하게 EC2 인스턴스에 Cloud9 이 설치됩니다.

6. 다시 Cloud9 콘솔로 돌아와서 구성을 완료합니다. 다음과 같이 IDE 가 바로 구성되어야 합니다.

![IQ](../../images/fleetiq/lab34/IQServer-3.png)


### 서버 사이드 스크립트 작성

1. Cloud9 환경에 server.py 파일을 생성하고, 다음 서버 스크립트를 복사하여 붙여넣습니다. 이 내부 코드를 하나하나 수정해볼 것입니다.

```python
import socket
import boto3
import requests


GameServerGroupName = 'GameServerGroups'
GameServerId = 'game-server-1'
InstanceId = requests.get('http://169.254.169.254/latest/meta-data/instance-id').text
ConnectionInfo = requests.get('http://169.254.169.254/latest/meta-data/public-hostname').text

client = boto3.client('gamelift', 'ap-northeast-2')

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5000))
server_socket.listen(5)

def list_game_servers():
    pass

# Register GameServer
def register_game_server():
    pass
#

# Update GameServer
def update_game_server(flag):
    pass
#

# Deregister GameServer
def deregister_game_server():
    pass
#

print("TCP server waiting for Client on port 5000")

while True:
    # List Game Server 
    listResponse = list_game_servers()

    if not 'GameServers' in listResponse or not any(server['GameServerId'] == GameServerId for server in listResponse['GameServers']):
        # Register Game Server to Game Server Group
        register_game_server()
        #

    # Update Game Server Status "AVAILABLE"
    update_game_server(True)
    #

    client_socket, address = server_socket.accept()
    print("I got connection from ", address)

    # Update Game Server Status "UTILIZED"
    update_game_server(False)
    #

    while True:
        data = client_socket.recv(512).decode()
        print("RECEIVED:", data)
        if(data == 'q' or data == 'Q'):
            client_socket.close()
            break
        else:
            client_socket.send(data.encode())

    # Deregister Game Server
    deregister_game_server()
    #

server_socket.close()
print("SOCKET Closed... End")

```

**먼저 위의 코드에서 리전을 실습을 진행하는 리전으로 변경해줍니다. 그리고 맨 위의 GameServerGroupName 을 앞서 구성하신 게임 서버 그룹 이름으로 변경합니다. GameServerId는 임의로 유니크하게 지정해줍니다**

2. 비어있는 각 함수 코드들에 내용을 채워넣습니다. 먼저 list_game_servers 함수입니다.

```python
def list_game_servers():
    response = client.list_game_servers(
        GameServerGroupName=GameServerGroupName
    )
    print(response)
    return response
```

이 함수는 현재 가용한 게임 서버 그룹 내의 게임 서버들을 확인합니다. 비어있다면 새로 등록해줘야 합니다.

3. 다음으로 게임 서버를 등록해줍니다. 다음 코드는 게임 서버 자신의 인스턴스를 서버 그룹에 등록하는 동작을 보여줍니다.

```python
def register_game_server():
    response = client.register_game_server(
        GameServerGroupName=GameServerGroupName,
        GameServerId=GameServerId,
        InstanceId=InstanceId,
        ConnectionInfo=ConnectionInfo
    )
    print(response)
```

4. update_game_server 함수는 서버 스스로 현재 가용 여부를 보고합니다. FleetIQ 의 HealthCheck 프로세스의 역할도 같이 수행하게 됩니다.

```python
def update_game_server(flag):
    if flag == True:
        return client.update_game_server(
            GameServerGroupName=GameServerGroupName,
            GameServerId=GameServerId,
            HealthCheck="HEALTHY",
            UtilizationStatus="AVAILABLE"
        )
    else:
        return client.update_game_server(
            GameServerGroupName=GameServerGroupName,
            GameServerId=GameServerId,
            HealthCheck="HEALTHY",
            UtilizationStatus="UTILIZED"
        )
```

5. deregister_game_server 함수는 게임 서버의 등록을 해제해주는 함수입니다. 본 예제에서는 단일 클라이언트를 처리하는 서버이므로 루프 이후에 해제되고 다음 연결에서 재등록되지만, 실제 구현에서는 클라이언트 핸들링 로직에 알맞게 게임 서버의 라이프사이클을 조절해줍니다.

```python
def deregister_game_server():
    response = client.deregister_game_server(
        GameServerGroupName=GameServerGroupName,
        GameServerId=GameServerId
    )
    print(response)
```

6. 완성된 코드는 다음과 같습니다.

```python
import socket
import boto3
import requests


GameServerGroupName = 'GameServerGroups'
GameServerId = 'game-server-1'
InstanceId = requests.get('http://169.254.169.254/latest/meta-data/instance-id').text
ConnectionInfo = requests.get('http://169.254.169.254/latest/meta-data/public-hostname').text

client = boto3.client('gamelift', 'ap-northeast-2')

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5000))
server_socket.listen(5)

def list_game_servers():
    response = client.list_game_servers(
        GameServerGroupName=GameServerGroupName
    )
    print(response)
    return response

# Register GameServer
def register_game_server():
    response = client.register_game_server(
        GameServerGroupName=GameServerGroupName,
        GameServerId=GameServerId,
        InstanceId=InstanceId,
        ConnectionInfo=ConnectionInfo
    )
    print(response)
#

# Update GameServer
def update_game_server(flag):
    if flag == True:
        return client.update_game_server(
            GameServerGroupName=GameServerGroupName,
            GameServerId=GameServerId,
            HealthCheck="HEALTHY",
            UtilizationStatus="AVAILABLE"
        )
    else:
        return client.update_game_server(
            GameServerGroupName=GameServerGroupName,
            GameServerId=GameServerId,
            HealthCheck="HEALTHY",
            UtilizationStatus="UTILIZED"
        )
#

# Deregister GameServer
def deregister_game_server():
    response = client.deregister_game_server(
        GameServerGroupName=GameServerGroupName,
        GameServerId=GameServerId
    )
    print(response)
#

print("TCP server waiting for Client on port 5000")

while True:
    # List Game Server 
    listResponse = list_game_servers()

    if not 'GameServers' in listResponse or not any(server['GameServerId'] == GameServerId for server in listResponse['GameServers']):
        # Register Game Server to Game Server Group
        register_game_server()
        #

    # Update Game Server Status "AVAILABLE"
    update_game_server(True)
    #

    client_socket, address = server_socket.accept()
    print("I got connection from ", address)

    # Update Game Server Status "UTILIZED"
    update_game_server(False)
    #

    while True:
        data = client_socket.recv(512).decode()
        print("RECEIVED:", data)
        if(data == 'q' or data == 'Q'):
            client_socket.close()
            break
        else:
            client_socket.send(data.encode())

    # Deregister Game Server
    deregister_game_server()
    #

server_socket.close()
print("SOCKET Closed... End")
```

7. 다음으로 클라이언트를 구현해보도록 하겠습니다.

---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
