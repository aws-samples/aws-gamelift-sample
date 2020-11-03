---
title: FleetIQ 기반 클라이언트 생성
url: /fleetiq/lab35
weight: 80
pre: "<b>3-5. </b>"
---

### Cloud9 기반의 FleetIQ 클라이언트 개발 환경 구축 

이번 실습에서는 Cloud 기반의 IDE 인 Cloud9 을 활용하여 간편하게 원격 개발환경을 구축하고 FleetIQ 예제를 위한 클라이언트를 생성해보겠습니다.

Cloud9 을 통해 별다른 로컬 환경의 변경이나 개발 환경 구성의 어려움 없이 일관성있는 온라인 통합 개발 환경을 구축하실 수 있습니다.

1. Cloud9 콘솔에 접속합니다. (https://ap-northeast-2.console.aws.amazon.com/cloud9/home/product)

2. 환경의 이름을 FleetIQClient 로 지정합니다.

![C9](../../images/fleetiq/lab35/C9-1.png)

3. EC2 환경으로 지정하고 적절한 인스턴스 타입을 선택합니다. 네트워크 세팅은 기본 VPC 에 위치하도록 구성합니다.

![CL](../../images/fleetiq/lab35/CL-1.png)

4. Cloud9 개발 환경이 만들어지는 것을 기다립니다. 몇 분 내에 작업은 완료됩니다.

5. Cloud9 IDE 가 실행되면, client.py 파일을 만들어주고, 다음 소스 코드를 붙여넣습니다.

```python
import socket
import boto3

client = boto3.client('gamelift')

# Claim Game Server
def claim_game_server():
    pass
#
response = claim_game_server()
#endpoint = response['GameServer']['ConnectionInfo']
endpoint = ''

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((endpoint, 5000))

while True:
    data = input("SEND(TYPE q or Q to Quit):")
    if(data == 'q' or data == 'Q'):
        client_socket.send(data.encode())
        client_socket.close()
        break
    else:
        client_socket.send(data.encode())
    data = client_socket.recv(512).decode()
    print("RECEIVED:", data)

print("Socket Closed... END")
```

**이 코드는 간단한 TCP 에코 클라이언트를 구현한 모습입니다. 하지만 자세히 보시면 서버의 엔드포인트가 지정되어있지 않습니다. 이제 이 부분을 구현해보겠습니다.**

6. 코드에서 claim_game_server 함수를 구현해주어야 합니다. claimGameServer API 는 GameLift FleetIQ 에게 클라이언트에게 게임 세션을 할당하기에 적합한 서버 인스턴스를 요청하는 API 입니다.    
클라이언트는 게임 서버에 연결하기 위해 해당 API 를 통해 요청을 구축해주기만 하면 됩니다.
claim_game_server 함수에 다음과 같은 코드를 구성합니다.

```python
def claim_game_server():
    response = client.claim_game_server(
        GameServerGroupName='GameServerGroups'
    )
    print(response)
    return response
```

함수의 리턴 값은 할당된 게임 서버의 엔드포인트 정보를 갖습니다. 기존 소스에 주석을 제거하고 endpoint 에 응답값을 다음과 같이 할당합니다.
```python
endpoint = response['GameServer']['ConnectionInfo']
```

7. 완성된 코드는 다음과 같습니다

```python
import socket
import boto3

client = boto3.client('gamelift', 'ap-northeast-2')

# Claim Game Server
def claim_game_server():
    response = client.claim_game_server(
        GameServerGroupName='GameServerGroups'
    )
    print(response)
    return response
#
response = claim_game_server()
endpoint = response['GameServer']['ConnectionInfo']

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((endpoint, 5000))

while True:
    data = input("SEND(TYPE q or Q to Quit):")
    if(data == 'q' or data == 'Q'):
        client_socket.send(data.encode())
        client_socket.close()
        break
    else:
        client_socket.send(data.encode())
    data = client_socket.recv(512).decode()
    print("RECEIVED:", data)

print("Socket Closed... END")
```

실습에서는 FleetIQ API 구동을 학습하기 위한 목적으로 간단한 TCP 연결만을 구성하지만, 코드가 간단하지만 보다 복잡한 게임 로직으로 실습을 진행하셔도 좋습니다.

**이제 이전에 만들었던 서버와 클라이언트가 어떻게 상호작용하는지 알아보도록 하겠습니다.**

---
<p align="left">
© 2020 Amazon Web Services, Inc. 또는 자회사, All rights reserved.
</p>
