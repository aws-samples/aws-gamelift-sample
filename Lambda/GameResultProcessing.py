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
dynamodb = boto3.resource('dynamodb', region_name='ap-northeast-1')

ddb_table = dynamodb.Table('GomokuPlayerInfo')
queue_url = 'https://sqs.ap-northeast-1.amazonaws.com/675961292922/game-result-queue'



def lambda_handler(event, context):
    for i in xrange(10):
        response = sqs.receive_message(QueueUrl=queue_url, MaxNumberOfMessages=1,  MessageAttributeNames=['All'], VisibilityTimeout=0,  WaitTimeSeconds=1)
        
        if 'Messages' not in response:
            break
            
        message = response['Messages'][0]
        parsed = json.loads(message['Body'])
        
        playername = parsed['PlayerName']
        scorediff = parsed['ScoreDiff']
        windiff = parsed['WinDiff']
        losediff = parsed['LoseDiff']
        ddb_table.update_item(
            Key={ 'PlayerName' : playername }, 
            UpdateExpression="set Score = Score + :score, Win = Win + :win, Lose = Lose + :lose",
            ExpressionAttributeValues={
                ':score': decimal.Decimal(scorediff),
                ':win': decimal.Decimal(windiff),
                ':lose': decimal.Decimal(losediff)
            },
            ReturnValues="UPDATED_NEW"
        )
        
        receipt_handle = message['ReceiptHandle']
        
        sqs.delete_message(QueueUrl=queue_url, ReceiptHandle=receipt_handle)

        

    

