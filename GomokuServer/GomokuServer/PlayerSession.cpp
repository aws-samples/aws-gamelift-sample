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
#include "GameLiftManager.h"

#include <aws/gamelift/server/GameLiftServerAPI.h>

#define CLIENT_BUFSIZE	65536

PlayerSession::PlayerSession(SOCKET sock) : Session(sock, CLIENT_BUFSIZE, CLIENT_BUFSIZE), mScore(0)
{
}

PlayerSession::~PlayerSession()
{
}

void PlayerSession::PlayerReady(const std::string& playerId)
{
    if (GGameLiftManager->AcceptPlayerSession(std::static_pointer_cast<PlayerSession>(shared_from_this()), playerId))
    {
        mPlayerSessionId = playerId;

        /// Score info from GL
        Aws::GameLift::Server::Model::DescribePlayerSessionsRequest req;
        req.SetPlayerSessionId(mPlayerSessionId);
        auto outcome = Aws::GameLift::Server::DescribePlayerSessions(req);
        if (!outcome.IsSuccess())
        {
            GConsoleLog->PrintOut(true, "[PLAYER] DescribePlayerSessions Error : %s \n", outcome.GetError().GetErrorMessage().c_str());
            mScore = -1000;
            mPlayerName = std::string("nonamed");
            return;
        }

        mPlayerName = outcome.GetResult().GetPlayerSessions()[0].GetPlayerId();
        mScore = GGameLiftManager->FindScoreFromMatchData(mPlayerName);

        GConsoleLog->PrintOut(true, "[PLAYER] PlayerReady: %s \n", playerId.c_str());

        GGameLiftManager->CheckReadyAll();

        return;
    }

    /// disconnect unauthed player
    Disconnect(DR_UNAUTH);
}

void PlayerSession::PlayerExit(const std::string& playerId)
{
    GGameLiftManager->RemovePlayerSession(std::static_pointer_cast<PlayerSession>(shared_from_this()), playerId);
    mPlayerSessionId.clear();

    GConsoleLog->PrintOut(true, "[PLAYER] PlayerExit: %s \n", playerId.c_str());

    Disconnect(DR_LOGOUT);
}

void PlayerSession::OnDisconnect(DisconnectReason dr)
{
    if (IsValid())
    {
        GGameLiftManager->RemovePlayerSession(std::static_pointer_cast<PlayerSession>(shared_from_this()), mPlayerSessionId);
        mPlayerSessionId.clear();
    }
}
