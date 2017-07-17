from __future__ import print_function
import boto3
import redis
import json

redis = redis.Redis(host='gomokuranking.nxaab2.0001.apne1.cache.amazonaws.com', port=6379, db=0)

def handler(event, context):
    result = redis.zrevrange('Rating', 0, -1, True)
    flat_list = [item for sublist in result for item in sublist]
    ret = []
    for i in range(0, len(flat_list)):
        if ( i % 2 == 0 ):
            org = json.loads(flat_list[i])
            org['Score'] = int(flat_list[i+1])
            ret.append(org)
            
    return ret

