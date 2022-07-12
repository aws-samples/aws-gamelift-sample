from __future__ import print_function
import boto3
import redis
import json
import os

endpoint = os.environ['REDIS']
redis = redis.Redis(host=endpoint, port=6379, db=0)

def handler(event, context):
    result = redis.zrevrange('Rating', 0, -1, True)
    print(result)
    flat_list = [item for sublist in result for item in sublist]
    ret = []
    for i in range(0, len(flat_list)):
        if ( i % 2 == 0 ):
            org = {}
            org['Player'] = flat_list[i].decode('utf-8')
            org['Score'] = int(flat_list[i+1])
            ret.append(org)
            
    return ret

