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
#include "Log.h"
#include "SharedStruct.h"
#include "PlayerSession.h"
#include "MatchMaker.h"

//@{ Handler Helper

typedef std::shared_ptr<PlayerSession> PlayerSessionPtr;

typedef void(*HandlerFunc)(PlayerSessionPtr session);

static HandlerFunc HandlerTable[PKT_MAX];

static void DefaultHandler(PlayerSessionPtr session)
{
	session->Disconnect(DR_ACTIVE);
}

struct InitializeHandlers
{
	InitializeHandlers()
	{
		for (int i = 0; i < PKT_MAX; ++i)
			HandlerTable[i] = DefaultHandler;
	}
} _init_handlers_;

struct RegisterHandler
{
	RegisterHandler(int pktType, HandlerFunc handler)
	{
		HandlerTable[pktType] = handler;
	}
};

#define REGISTER_HANDLER(PKT_TYPE)	\
	static void Handler_##PKT_TYPE(PlayerSessionPtr session); \
	static RegisterHandler _register_##PKT_TYPE(PKT_TYPE, Handler_##PKT_TYPE); \
	static void Handler_##PKT_TYPE(PlayerSessionPtr session)

//@}


void PlayerSession::OnRead(size_t len)
{
	/// packet parsing
	while (true)
	{
		/// read a packet header
		PacketHeader header;
		if (false == mRecvBuffer.Peek((char*)&header, sizeof(PacketHeader)))
			return;

		/// check a packet completed on not
		if (mRecvBuffer.GetStoredSize() < header.mSize)
			return;


		if (header.mType >= PKT_MAX || header.mType <= PKT_NONE)
		{
			Disconnect(DR_ACTIVE);
			return;
		}

		/// packet dispatch...
		HandlerTable[header.mType](std::static_pointer_cast<PlayerSession>(shared_from_this()));
	}
}

/////////////////////////////////////////////////////////
REGISTER_HANDLER(PKT_CM_MATCH_REQUEST)
{
	MatchRequest inPacket;
	if (false == session->ParsePacket(inPacket))
	{
		GConsoleLog->PrintOut(true, "packet parsing error: %d\n", inPacket.mType);
		return;
	}

	MatchWait outPacket;

	if (!GMatchMaker->PlayerLogin(session, std::string(inPacket.mPlayerName), std::string(inPacket.mPlayerPass)))
	{
		GConsoleLog->PrintOut(true, "Player Login Fail: %s\n", inPacket.mPlayerName);
		outPacket.mIsOK = false;
	}
	else
	{
		outPacket.mIsOK = GMatchMaker->RequestMatch(session);
	}

	if (false == session->PostSend((const char*)&outPacket, outPacket.mSize))
	{
		session->Disconnect(DR_SENDBUFFER_ERROR);
	}
}


