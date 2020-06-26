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
#include "GameSession.h"
#include "GameLiftManager.h"

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
REGISTER_HANDLER(PKT_CS_START)
{
    StartRequest inPacket;
    if (false == session->ParsePacket(inPacket))
    {
        GConsoleLog->PrintOut(true, "packet parsing error: %d\n", inPacket.mType);
        return;
    }

    session->PlayerReady(std::string(inPacket.mPlayerId));
}

REGISTER_HANDLER(PKT_CS_EXIT)
{
    ExitRequest inPacket;
    if (false == session->ParsePacket(inPacket))
    {
        GConsoleLog->PrintOut(true, "packet parsing error: %d\n", inPacket.mType);
        return;
    }

    session->PlayerExit(std::string(inPacket.mPlayerId));
}

REGISTER_HANDLER(PKT_CS_PUT_STONE)
{
    PutStoneRequest inPacket;
    if (false == session->ParsePacket(inPacket))
    {
        GConsoleLog->PrintOut(true, "packet parsing error: %d\n", inPacket.mType);
        return;
    }

    GGameLiftManager->GetGameSession()->PutStone(session, inPacket.mPosX, inPacket.mPosY);
}


