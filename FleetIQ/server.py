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
