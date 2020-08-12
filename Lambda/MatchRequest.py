from __future__ import print_function
import boto3
import sys
import json
import decimal

# Helper class to convert a DynamoDB item to JSON.
class DecimalEncoder(json.JSONEncoder):
    def default(self, o):
        if isinstance(o, decimal.Decimal):
            if o % 1 > 0:
                return float(o)
            else:
                return int(o)
        return super(DecimalEncoder, self).default(o)
        

gl_client = boto3.client('gamelift')
dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

ddb_table = dynamodb.Table('GomokuPlayerInfo')

def lambda_handler(event, context):
    playerId = event['PlayerName']
    playerPass = event['PlayerPass']
    playerScore = -1
    
    response = {'TicketId' : 'AuthError' }
    
    result = ddb_table.get_item( Key= {'PlayerName' : playerId } )
 
    if 'Item' not in result:
        # Create Item
        ddb_table.put_item( Item={ 'PlayerName' : playerId, 'Password' : playerPass, 'Score' : 1000, 'Win' : 0, 'Lose' : 0} )
        result = ddb_table.get_item( Key= {'PlayerName' : playerId } )
        playerScore = 1000
    elif result['Item']['Password'] != playerPass:
        print(result)
        return response
    
    playerScore = result['Item']['Score']
    playerAttr = {'score': {'N': int(playerScore) }}
    
    # Auth OK, Match Request Go
    try:
        match_response = gl_client.start_matchmaking(
            ConfigurationName='GomokuMatchConfig',
            Players = [ { 'PlayerId' : playerId, 'PlayerAttributes' : playerAttr } ]
            )
    except TypeError as e:
        print(e)
        response = {'TicketId' : 'MatchError' }
        return response
    except:
        print(sys.exc_info()[0])
        response = {'TicketId' : 'MatchError' }
        return response

    ticketId = match_response['MatchmakingTicket']['TicketId']

    response = {'TicketId' : ticketId } 
    return response

