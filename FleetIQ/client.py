

import socket
import boto3

client = boto3.client('gamelift')

# Claim Game Server
def claim_game_server():
    response = client.claim_game_server(
        GameServerGroupName='GomokuServerGroups',
        GameServerId='GomokuServer-1'
    )
    print(response)
    return response
#
response = claim_game_server()
endpoint = response['GameServer']['ConnectionInfo']

client_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
client_socket.connect((endpoint, 5000))

while True:
    data = client_socket.recv(512).decode()
    if(data == 'q' or data == 'Q'):
        client_socket.close()
        break
    else:
        print("RECEIVED:", data)
        data = input("SEND(TYPE q or Q to Quit):")
        if(data == 'q' or data == 'Q'):
            client_socket.send(data.encode())
            client_socket.close()
            break
        else:
            client_socket.send(data.encode())

print("Socket Closed... END")
