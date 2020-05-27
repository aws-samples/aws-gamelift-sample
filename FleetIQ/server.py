import socket
import boto3

GameServerGroupName = 'GomokuServerGroups'
GameServerId = 'GomokuServer-1'
InstanceId = 'i-0b9bd2e4a0252cfb5'
ConnectionInfo = 'ec2-3-233-226-183.compute-1.amazonaws.com'

client = boto3.client('gamelift', 'us-east-1')

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind(("", 5000))
server_socket.listen(5)

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
def update_game_server():
    pass
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
    # Register Game Server to Game Server Group
    register_game_server()

    client_socket, address = server_socket.accept()
    print("I got connection from ", address)

    # Update Game Server Status
    update_game_server()

    while True:
        data = input('SEND(TYPE q or Q to Quit:')
        if(data == 'Q' or data == 'q'):
            client_socket.send(data.encode())
            client_socket.close()
            break
        else:
            client_socket.send(data.encode())
        
        data = client_socket.recv(512).decode()
        if(data == 'q' or data == 'Q'):
            client_socket.close()
            break
        else:
            print("RECEIVED:", data)
    break

# Deregister Game Server
deregister_game_server()
#
server_socket.close()
print("SOCKET Closed... End")
