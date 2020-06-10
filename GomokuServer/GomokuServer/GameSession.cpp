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

GameSession::GameSession() : mGameStatus(GameStatus::GS_NOT_STARTED), mCurrentTurn(StoneType::STONE_NONE)
{}

void GameSession::PlayerEnter(std::shared_ptr<PlayerSession> psess)
{
    FastSpinlockGuard lock(mGameSessionLock);

    if (mGameStatus != GameStatus::GS_NOT_STARTED)
    {
        GConsoleLog->PrintOut(true, "[PlayerEnter Denied] Game has already started.\n", psess->GetPlayerSessionId().c_str());
        return;
    }

    if (mPlayerBlack)
    {
        /// Game Ready!
        mPlayerWhite = psess;
        mGameStatus = GameStatus::GS_STARTED;
        mCurrentTurn = StoneType::STONE_BLACK;
    }
    else
    {
        mPlayerBlack = psess;
    }
}

void GameSession::PlayerLeave(std::shared_ptr<PlayerSession> psess)
{
    FastSpinlockGuard lock(mGameSessionLock);

    if (mGameStatus == GameStatus::GS_STARTED)
    {
        /// giveup
        if (psess == mPlayerBlack)
        {
            mGameStatus = GameStatus::GS_GAME_OVER_WHITE_WIN;
            SendGameResult(false);
        }
        else
        {
            mGameStatus = GameStatus::GS_GAME_OVER_BLACK_WIN;
            SendGameResult(true);
        }

        BroadcastGameStatus();
    }

    if (psess == mPlayerBlack)
    {
        mPlayerBlack.reset();
    }
    else
    {
        mPlayerWhite.reset();
    }
}

void GameSession::PutStone(std::shared_ptr<PlayerSession> psess, int x, int y)
{
    if (x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE)
    {
        GConsoleLog->PrintOut(true, "[PutStone Denied] out of range\n", psess->GetPlayerSessionId().c_str());
        return;
    }

    if (mGameStatus != GameStatus::GS_STARTED)
    {
        GConsoleLog->PrintOut(true, "[PutStone Denied] Not started game\n", psess->GetPlayerSessionId().c_str());
        return;
    }

    FastSpinlockGuard lock(mGameSessionLock);


    bool isBlack = (psess == mPlayerBlack);

    if (isBlack && mCurrentTurn != StoneType::STONE_BLACK)
    {
        GConsoleLog->PrintOut(true, "[PutStone Denied] Turn mismatch\n", psess->GetPlayerSessionId().c_str());
        return;
    }

    if (!isBlack && mCurrentTurn != StoneType::STONE_WHITE)
    {
        GConsoleLog->PrintOut(true, "[PutStone Denied] Turn mismatch\n", psess->GetPlayerSessionId().c_str());
        return;
    }

    if (mBoardStatus.mBoardMatrix[x][y] != StoneType::STONE_NONE)
    {
        GConsoleLog->PrintOut(true, "[PutStone Denied] wrong position\n", psess->GetPlayerSessionId().c_str());
        return;
    }

    StoneType st = isBlack ? StoneType::STONE_BLACK : StoneType::STONE_WHITE;
    mBoardStatus.mBoardMatrix[x][y] = st;

    /// Win check...
    if (IsWin(st))
    {
        mGameStatus = isBlack ? GameStatus::GS_GAME_OVER_BLACK_WIN : GameStatus::GS_GAME_OVER_WHITE_WIN;
        SendGameResult(isBlack);
    }

    if (isBlack)
        mCurrentTurn = StoneType::STONE_WHITE;
    else
        mCurrentTurn = StoneType::STONE_BLACK;

    BroadcastGameStatus();
}


void GameSession::BroadcastGameStart()
{
    CRASH_ASSERT(mGameStatus == GameStatus::GS_STARTED);

    GameStartBroadcast outPacketToBlack;

    strcpy_s(outPacketToBlack.mFirstPlayerId, mPlayerBlack->GetPlayerSessionId().c_str());
    strcpy_s(outPacketToBlack.mOpponentName, mPlayerWhite->GetPlayerName().c_str());

    if (false == mPlayerBlack->PostSend((const char*)&outPacketToBlack, outPacketToBlack.mSize))
    {
        mPlayerBlack->Disconnect(DR_SENDBUFFER_ERROR);
    }


    GameStartBroadcast outPacketToWhite;
    strcpy_s(outPacketToWhite.mFirstPlayerId, mPlayerBlack->GetPlayerSessionId().c_str());
    strcpy_s(outPacketToWhite.mOpponentName, mPlayerBlack->GetPlayerName().c_str());

    if (false == mPlayerWhite->PostSend((const char*)&outPacketToWhite, outPacketToWhite.mSize))
    {
        mPlayerWhite->Disconnect(DR_SENDBUFFER_ERROR);
    }
}

void GameSession::BroadcastGameStatus()
{
    BoardStatusBroadcast outPacket;
    outPacket.mBoardStatus = mBoardStatus;
    outPacket.mGameStatus = mGameStatus;
    outPacket.mCurrentTurn = mCurrentTurn;

    if (false == mPlayerBlack->PostSend((const char*)&outPacket, outPacket.mSize))
    {
        mPlayerBlack->Disconnect(DR_SENDBUFFER_ERROR);
    }

    if (false == mPlayerWhite->PostSend((const char*)&outPacket, outPacket.mSize))
    {
        mPlayerWhite->Disconnect(DR_SENDBUFFER_ERROR);
    }
}

bool GameSession::IsWin(StoneType st)
{
    for (int l = 0; l < BOARD_SIZE; ++l)
    {
        for (int i1 = 0; i1 < BOARD_SIZE; ++i1)
        {
            if (l < BOARD_SIZE - 4 && CheckLine(st, l, i1, 1, 0))
                return true;

            if (l < BOARD_SIZE - 4 && i1 < BOARD_SIZE - 4 && CheckLine(st, l, i1, 1, 1))
                return true;

            if (i1 < BOARD_SIZE - 4 && CheckLine(st, l, i1, 0, 1))
                return true;

            if (l <= 3 || i1 >= BOARD_SIZE - 4 || !CheckLine(st, l, i1, -1, 1))
                continue;

            return true;
        }
    }

    return false;
}

bool GameSession::CheckLine(StoneType st, int i, int j, int l, int i1)
{
    int j1 = 0;

    do
    {
        if (mBoardStatus.mBoardMatrix[i + j1*l][j + j1*i1] != st)
            return false;
    } while (++j1 < 5);

    return true;
}

int GameSession::CalcEloScore(int myScore, int opponentScore, bool win) const
{
    const int K = 100;
    double expected = 1 / (1 + ::pow(10, ((myScore - opponentScore) / 400)));
    double result = std::round(myScore + K * (win ? 1 : 0 - expected));
    return static_cast<int>(result) - myScore;
}

std::string GameSession::MakeResultJsonString(const std::string& playerName, int scorediff, int windiff, int losediff) const
{
    std::stringstream ss;
    ss << "{ " << std::quoted("PlayerName") << " : " << std::quoted(playerName) << ", "
        << std::quoted("WinDiff") << " : " << windiff << ", "
        << std::quoted("LoseDiff") << " : " << losediff << ", "
        << std::quoted("ScoreDiff") << " : " << scorediff << " }";

    return ss.str();
}

void GameSession::SendGameResult(bool isBlackWin) const
{
    if (isBlackWin)
        GConsoleLog->PrintOut(true, "[GAME OVER] Player %s Win!\n", mPlayerBlack->GetPlayerSessionId().c_str());
    else
        GConsoleLog->PrintOut(true, "[GAME OVER] Player %s Win!\n", mPlayerWhite->GetPlayerSessionId().c_str());

    int blackNew = CalcEloScore(mPlayerBlack->GetPlayerScore(), mPlayerWhite->GetPlayerScore(), isBlackWin);
    int whiteNew = CalcEloScore(mPlayerWhite->GetPlayerScore(), mPlayerBlack->GetPlayerScore(), !isBlackWin);

    auto blackJson = MakeResultJsonString(mPlayerBlack->GetPlayerName(), blackNew, isBlackWin ? 1 : 0, isBlackWin ? 0 : 1);
    auto whiteJson = MakeResultJsonString(mPlayerWhite->GetPlayerName(), whiteNew, isBlackWin ? 0 : 1, isBlackWin ? 1 : 0);

    /// Send to SQS
    GGameLiftManager->SendGameResultToSQS(blackJson, whiteJson);

}

