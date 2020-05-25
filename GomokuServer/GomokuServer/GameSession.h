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
#include "FastSpinlock.h"
#include "SharedStruct.h"
class PlayerSession;
struct PacketHeader;
class GameLiftManager;

class GameSession
{
public:
    GameSession();

    bool IsEnd() const
    {
        return mGameStatus == GameStatus::GS_GAME_OVER_BLACK_WIN
            || mGameStatus == GameStatus::GS_GAME_OVER_WHITE_WIN;
    }

    void PutStone(std::shared_ptr<PlayerSession> psess, int x, int y);

private:

    void BroadcastGameStart();
    void BroadcastGameStatus();
    bool IsWin(StoneType st);
    bool CheckLine(StoneType st, int i, int j, int l, int i1);

    int CalcEloScore(int myScore, int opponentScore, bool win) const;
    std::string MakeResultJsonString(const std::string& playerName, int scorediff, int windiff, int losediff) const;
    void SendGameResult(bool isBlackWin) const;

    GameStatus mGameStatus;
    BoardStatus mBoardStatus;
    StoneType mCurrentTurn;
    FastSpinlock mGameSessionLock;

private:
    void PlayerEnter(std::shared_ptr<PlayerSession> psess);
    void PlayerLeave(std::shared_ptr<PlayerSession> psess);

    std::shared_ptr<PlayerSession> mPlayerBlack;
    std::shared_ptr<PlayerSession> mPlayerWhite;

    friend class GameLiftManager;
};
