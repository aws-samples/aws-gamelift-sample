from __future__ import print_function
import boto3
import redis
import json
import os

endpoint = os.environ['REDIS']
redis = redis.Redis(host=endpoint, port=6379, db=0)

def handler(event, context):
    for record in event['Records']:
        if record['eventName'] == "REMOVE":
            continue
        
        print(record)
        playerName = record['dynamodb']['NewImage']['PlayerName']['S']
        
        if 'OldImage' in record['dynamodb'].keys():
            oldWin = int(record['dynamodb']['OldImage']['Win']['N'])
            oldLose = int(record['dynamodb']['OldImage']['Lose']['N'])
            
            olddata = { "PlayerName" : str(playerName), "Win" : int(oldWin), "Lose" : int(oldLose)}
            old_json = json.dumps(olddata)
            redis.zrem('Rating', old_json)
        
        newScore = int(record['dynamodb']['NewImage']['Score']['N'])
        redis.zadd('Rating', str(playerName), float(newScore))

    return "OK" 
