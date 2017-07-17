/*
* Copyright 2017 Amazon.com, Inc. or its affiliates. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License").
* You may not use this file except in compliance with the License.
* A copy of the License is located at
*
*  http://aws.amazon.com/apache2.0
*
* or in the "license" file accompanying this file. This file is distributed
* on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either
* express or implied. See the License for the specific language governing
* permissions and limitations under the License.
*/

#include "stdafx.h"
#include "PlayerSession.h"
#include "Log.h"
#include "SharedStruct.h"


#define CLIENT_BUFSIZE	65536

PlayerSession::PlayerSession(SOCKET sock) : Session(sock, CLIENT_BUFSIZE, CLIENT_BUFSIZE), mScore(0)
{
}

PlayerSession::~PlayerSession()
{
}


void PlayerSession::SendMatchInfo(int port, const std::string& ipAddr, const std::string& psessId)
{
	MatchResult outPacket;
	outPacket.mPort = port;
	sprintf_s(outPacket.mIpAddress, "%s", ipAddr.c_str());
	sprintf_s(outPacket.mPlayerId, "%s", psessId.c_str());

	if (false == PostSend((const char*)&outPacket, outPacket.mSize))
	{
		Disconnect(DR_SENDBUFFER_ERROR);
	}
}