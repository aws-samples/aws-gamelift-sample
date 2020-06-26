

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
