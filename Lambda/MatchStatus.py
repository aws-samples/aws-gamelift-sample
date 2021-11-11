import boto3
from botocore.exceptions import ClientError
import json
import time

dynamodb = boto3.resource('dynamodb', region_name='us-east-1')
ddb_table = dynamodb.Table('GomokuPlayerInfo')

def lambda_handler(event, context):
    print(event)
    
    # You can also use TicketId to track Matchmaking Event.
    ticket_id = event['TicketId']
    player_name = event['PlayerName']
    response = { 'IpAddress': '', 'PlayerSessionId': '', 'Port': 0 }
    try:
        match_response = ddb_table.get_item(
            TableName='GomokuPlayerInfo',
            Key={
                'PlayerName': player_name
            })
        if 'Item' in match_response:
            print(match_response['Item'])
            connection_info = json.loads(match_response['Item']['ConnectionInfo'])
            if connection_info['status'] == 'matching':
                response['IpAddress'] = connection_info['IpAddress']
                response['Port'] = connection_info['Port']
                response['PlayerSessionId'] = connection_info['PlayerSessionId']
                
                connection_update = { 'IpAddress': connection_info['IpAddress'], 'Port': connection_info['Port'], 'PlayerSessionId': connection_info['PlayerSessionId'], 'timestamp': int(time.time()), 'status': 'complete' }
                ddb_table.update_item(
                    TableName="GomokuPlayerInfo",
                    Key={ 'PlayerName' : player_name }, 
                    UpdateExpression="set ConnectionInfo = :connection_update",
                    ExpressionAttributeValues={
                        ':connection_update': "" + json.dumps(connection_update),
                    },
                    ReturnValues="UPDATED_NEW"
                )

    except ClientError as e:
        print(e.response['Error']['Message'])

    print(response)
    return response

