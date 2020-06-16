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

#pragma once

#include <aws/gamelift/server/model/GameSession.h>
#include "FastSpinLock.h"

struct PacketHeader;
class GameSession;
class PlayerSession;

class GameLiftManager
{
public:
    GameLiftManager();

    bool InitializeGameLift(int listenPort);

    void SetSQSClientInfo(const std::string& region, const std::string& url, const std::string& role);
    void SendGameResultToSQS(const std::string& blackJson, const std::string& whiteJson) const;

    void FinalizeGameLift();

    bool AcceptPlayerSession(std::shared_ptr<PlayerSession> psess, const std::string& playerSessionId);
    void RemovePlayerSession(std::shared_ptr<PlayerSession> psess, const std::string& playerSessionId);

    void OnStartGameSession(Aws::GameLift::Server::Model::GameSession myGameSession);

    void OnProcessTerminate();

    bool OnHealthCheck() { return mActivated; }

    std::shared_ptr<GameSession> GetGameSession() { return mGameSession; }

    void CheckReadyAll();

    int FindScoreFromMatchData(const std::string& playerName) const;

private:
    void TerminateGameSession(int exitCode);

private:
    FastSpinlock mLock;

    bool mActivated;
    int mCheckTerminationCount;

    std::string mSQSUrl;
	std::string mSQSRole;
    std::string mSQSRegion;

    std::shared_ptr<GameSession> mGameSession; ///< 1:1 relationship, currently
    volatile long mPlayerReadyCount;

    std::string mMatchMakerData;
};

extern std::unique_ptr<GameLiftManager> GGameLiftManager;
