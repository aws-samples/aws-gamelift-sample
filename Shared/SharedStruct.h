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

#ifndef WIN32
#include <string.h>
#endif // !WIN32


enum class StoneType : unsigned char
{
	STONE_NONE = 0,
	STONE_WHITE = 1,
	STONE_BLACK = 2
};

enum class GameStatus : unsigned char
{
	GS_NOT_STARTED,
	GS_STARTED,
	GS_GAME_OVER_BLACK_WIN,
	GS_GAME_OVER_WHITE_WIN
};

#define BOARD_SIZE	19

#define MAX_SESSION_LEN	128
#define MAX_STRING_LEN 64
#define MAX_PLAYER_PER_GAME	2


enum PacketTypes
{
	PKT_NONE = 0,

	/// Client and GameServer
	PKT_CS_START = 1,
	PKT_SC_START = 2,

	PKT_CS_PUT_STONE = 21,
	PKT_SC_BOARD_STATUS = 22,

	PKT_CS_EXIT = 31,

	/// Client and MatchMaker
	PKT_CM_MATCH_REQUEST = 101,
	PKT_MC_WAIT	= 102,
	PKT_MC_MATCH_RESULT = 103,

	PKT_MAX = 1024
};

#pragma pack(push, 1)

struct PacketHeader
{
	PacketHeader() : mSize(0), mType(PKT_NONE) {}
	short mSize;
	short mType;
};


struct BoardStatus
{
	BoardStatus()
	{
		memset(mBoardMatrix, 0, sizeof(mBoardMatrix));
	}
	StoneType mBoardMatrix[BOARD_SIZE][BOARD_SIZE];
};


struct StartRequest : public PacketHeader
{
	StartRequest()
	{
		mSize = sizeof(StartRequest);
		mType = PKT_CS_START;
		memset(mPlayerId, 0, sizeof(mPlayerId));
	}

	char mPlayerId[MAX_SESSION_LEN];
};

struct GameStartBroadcast : public PacketHeader
{
	GameStartBroadcast()
	{
		mSize = sizeof(GameStartBroadcast);
		mType = PKT_SC_START;

		memset(mFirstPlayerId, 0, sizeof(mFirstPlayerId));
		memset(mOpponentName, 0, sizeof(mOpponentName));
	}

	char mFirstPlayerId[MAX_SESSION_LEN];
	char mOpponentName[MAX_STRING_LEN];
	
};

struct PutStoneRequest : public PacketHeader
{
	PutStoneRequest()
	{
		mSize = sizeof(PutStoneRequest);
		mType = PKT_CS_PUT_STONE;
		mPosX = 0;
		mPosY = 0;
	}

	int	mPosX;
	int	mPosY;
};

struct BoardStatusBroadcast : public PacketHeader
{
	BoardStatusBroadcast()
	{
		mSize = sizeof(BoardStatusBroadcast);
		mType = PKT_SC_BOARD_STATUS;
		mGameStatus = GameStatus::GS_NOT_STARTED;
		mCurrentTurn = StoneType::STONE_NONE;
	}

	BoardStatus mBoardStatus;
	GameStatus mGameStatus;
	StoneType mCurrentTurn;
};


struct ExitRequest : public PacketHeader
{
	ExitRequest()
	{
		mSize = sizeof(ExitRequest);
		mType = PKT_CS_EXIT;

		memset(mPlayerId, 0, sizeof(mPlayerId));
	}

	char mPlayerId[MAX_SESSION_LEN];

};

//////////////////////////////////////////////////////////////

struct MatchRequest : public PacketHeader
{
	MatchRequest()
	{
		mSize = sizeof(MatchRequest);
		mType = PKT_CM_MATCH_REQUEST;

		memset(mPlayerName, 0, sizeof(mPlayerName));
		memset(mPlayerPass, 0, sizeof(mPlayerPass));
	}

	char mPlayerName[MAX_STRING_LEN];
	char mPlayerPass[MAX_STRING_LEN];
};


struct MatchWait : public PacketHeader
{
	MatchWait()
	{
		mSize = sizeof(MatchWait);
		mType = PKT_MC_WAIT;

		mIsOK = false;
	}

	bool mIsOK;
};

struct MatchResult : public PacketHeader
{
	MatchResult()
	{
		mSize = sizeof(MatchResult);
		mType = PKT_MC_MATCH_RESULT;

		memset(mPlayerId, 0, sizeof(mPlayerId));
		memset(mIpAddress, 0, sizeof(mIpAddress));
		mPort = 0;
	}

	char mPlayerId[MAX_SESSION_LEN];
	char mIpAddress[MAX_STRING_LEN];
	int mPort;
};



#pragma pack(pop)