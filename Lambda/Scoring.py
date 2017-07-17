from __future__ import print_function
import boto3
import redis
import json

redis = redis.Redis(host='gomokuranking.nxaab2.0001.apne1.cache.amazonaws.com', port=6379, db=0)

def handler(event, context):
    for record in event['Records']:
        if record['eventName'] == "REMOVE":
            continue
        
        print(record)
        playerName = record['dynamodb']['NewImage']['PlayerName']['S']
        
        if 'OldImage' in record['dynamodb'].keys():
            oldScore = int(record['dynamodb']['OldImage']['Score']['N'])
            oldWin = int(record['dynamodb']['OldImage']['Win']['N'])
            oldLose = int(record['dynamodb']['OldImage']['Lose']['N'])
            
            olddata = { "PlayerName" : str(playerName), "Win" : int(oldWin), "Lose" : int(oldLose)}
            old_json = json.dumps(olddata)
            redis.zrem('Rating', old_json)
        
        newScore = int(record['dynamodb']['NewImage']['Score']['N'])
        newWin = int(record['dynamodb']['NewImage']['Win']['N'])
        newLose = int(record['dynamodb']['NewImage']['Lose']['N'])
        
        newdata = { "PlayerName" : str(playerName), "Win" : int(newWin), "Lose" : int(newLose)}
        new_json = json.dumps(newdata)
        
        redis.zadd('Rating', new_json, newScore)

    return "OK" 

