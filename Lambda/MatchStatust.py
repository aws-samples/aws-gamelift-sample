from __future__ import print_function
import boto3

client = boto3.client('gamelift')

def handler(event, context):
    playerId = event['PlayerName'] 
    ticketId = event['TicketId']
    
    response = { 'IpAddress' : 'NotYet', 'PlayerSessionId' : 'NotYet', 'Port' : 0 }
    
    match_response = client.describe_matchmaking( TicketIds = [ ticketId ] )
    match_ticket = match_response['TicketList'][0]
    print(match_ticket['Status'])
    if match_ticket['Status'] == 'COMPLETED':
        ipaddr = match_ticket['GameSessionConnectionInfo']['IpAddress']
        port = match_ticket['GameSessionConnectionInfo']['Port']
        for psess in match_ticket['GameSessionConnectionInfo']['MatchedPlayerSessions']:
            #print(psess['PlayerId'])
            sessionId = psess['PlayerSessionId']
            response = { 'IpAddress' : ipaddr, 'Port' : port, 'PlayerSessionId' : sessionId }
            break
        
    return response
