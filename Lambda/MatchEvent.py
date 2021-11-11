import boto3
import json
import time
import datetime

dynamodb = boto3.resource('dynamodb', region_name='us-east-1')
ddb_table = dynamodb.Table('GomokuPlayerInfo')

def lambda_handler(event, context):
    print(event)

    sns_message = json.loads(event['Records'][0]['Sns']['Message'])
    matchevent_status = sns_message['detail']['type']
    if matchevent_status == 'MatchmakingSucceeded':
        gamesession_info = sns_message['detail']['gameSessionInfo']

        address = gamesession_info['ipAddress']
        port = int(gamesession_info['port'])
        players = gamesession_info['players']
        
        for player in players:
            player_id = player['playerId']
            
            if 'playerSessionId' in player:
                player_session_id = player['playerSessionId']
                connection_info = { 'IpAddress': address, 'Port': port, 'PlayerSessionId': player_session_id, 'timestamp': int(time.time()), 'status': 'matching' }
                ddb_table.update_item(
                    TableName="GomokuPlayerInfo",
                    Key={ 'PlayerName' : player_id }, 
                    UpdateExpression="set ConnectionInfo = :connection_info",
                    ExpressionAttributeValues={
                        ':connection_info': "" + json.dumps(connection_info),
                    },
                    ReturnValues="UPDATED_NEW"
                )
    return {
        'statusCode': 200,
        'body': json.dumps('Hello from Lambda!')
    }