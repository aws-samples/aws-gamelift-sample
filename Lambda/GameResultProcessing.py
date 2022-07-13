from __future__ import print_function

import boto3
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
        
        
sqs = boto3.client('sqs')
dynamodb = boto3.resource('dynamodb', region_name='us-east-1')

ddb_table = dynamodb.Table('GomokuPlayerInfo')

def lambda_handler(event, context):
    print(event)
    for record in event['Records']:
        parsed = json.loads(record['body'])
        playername = parsed['PlayerName']
        scorediff = parsed['ScoreDiff']
        windiff = parsed['WinDiff']
        losediff = parsed['LoseDiff']
        response = ddb_table.update_item(
            Key={ 'PlayerName' : playername },
            UpdateExpression="SET Score = if_not_exists(Score, :basescore) + :score, Win = if_not_exists(Win, :basewin) + :win, Lose = if_not_exists(Lose, :baselose) + :lose",
            ExpressionAttributeValues={
                ':basescore': 1000,
                ':basewin': 0,
                ':baselose': 0,
                ':score': scorediff,
                ':win': windiff,
                ':lose': losediff
            }
        )
