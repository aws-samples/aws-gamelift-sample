from __future__ import print_function
import boto3
import redis
import json
import os

endpoint = os.environ['REDIS']
redis = redis.Redis(host=endpoint, port=6379, db=0)

def handler(event, context):
    for record in event['Records']:
        print(record)
        playerName = record['dynamodb']['Keys']['PlayerName']['S']

        if record['eventName'] == "REMOVE":
            redis.zrem('Rating', playerName)
        elif 'Score' in record['dynamodb']['NewImage']:
            newScore = int(record['dynamodb']['NewImage']['Score']['N'])
            redis.zadd('Rating', { playerName: newScore })

    return "OK" 
