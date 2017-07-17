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
#include "SharedStruct.h"
#include "Log.h"
#include "MatchMaker.h"
#include "PlayerSession.h"

#include <aws/core/Aws.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/core/utils/Outcome.h>
#include <aws/gamelift/model/SearchGameSessionsRequest.h>
#include <aws/gamelift/model/CreateGameSessionRequest.h>
#include <aws/gamelift/model/CreatePlayerSessionsRequest.h>

#include <aws/core/utils/memory/stl/AWSSet.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/client/CoreErrors.h>
#include <aws/core/auth/AWSCredentialsProviderChain.h>
#include <aws/core/http/HttpTypes.h>
#include <aws/dynamodb/DynamoDBClient.h>
#include <aws/dynamodb/DynamoDBErrors.h>
#include <aws/core/utils/UnreferencedParam.h>
#include <aws/core/utils/memory/stl/AWSSet.h>
#include <aws/core/utils/memory/stl/AWSStringStream.h>
#include <aws/core/utils/ratelimiter/DefaultRateLimiter.h>
#undef IN
#include <aws/dynamodb/model/PutItemRequest.h>
//#define IN
#include <aws/dynamodb/model/GetItemRequest.h>


std::unique_ptr<MatchMaker> GMatchMaker;


using namespace Aws;
using namespace Aws::Auth;
using namespace Aws::Http;
using namespace Aws::Client;
using namespace Aws::DynamoDB;
using namespace Aws::DynamoDB::Model;


MatchMaker::MatchMaker(const std::string& alias, const std::string& playerTable) : mAliasId(alias), mPlayerTableName(playerTable)
{
}

void MatchMaker::SetUpAwsClient(const std::string& region)
{
	Aws::Client::ClientConfiguration config;
	config.scheme = Aws::Http::Scheme::HTTPS;
	config.connectTimeoutMs = 10000;
	config.requestTimeoutMs = 10000;

	config.region = region;
	
	mDDBClient = Aws::MakeShared<Aws::DynamoDB::DynamoDBClient>("GameLiftMatchMaker", config);

	/// In case of GameLift Local
	if (mAliasId == "TEST_LOCAL")
	{
		config.scheme = Aws::Http::Scheme::HTTP;
		config.endpointOverride = "127.0.0.1:9080";
	}

	mGLClient = Aws::MakeShared<Aws::GameLift::GameLiftClient>("GameLiftMatchMaker", config);

}


bool MatchMaker::RequestMatch(std::shared_ptr<PlayerSession> psess)
{
	FastSpinlockGuard lock(mMatchLock);

	/// check if already exists (idempotention check)
	auto it = mMatchMap.find(psess->GetPlayerName());
	if (it == mMatchMap.end())
	{
		mMatchMap[psess->GetPlayerName()] = psess;
		return true;
	}

	return false;
}

void MatchMaker::DoMatchMaking()
{

	std::thread matchThread([=] 
	{
		while (true)
		{
			std::shared_ptr<PlayerSession> player1;
			std::shared_ptr<PlayerSession> player2;
			
			mMatchLock.EnterReadLock();
			if (mMatchMap.size() >= MAX_PLAYER_PER_GAME)
			{
				auto it = mMatchMap.begin();
				player1 = it->second;
				++it;
				player2 = it->second;
			}
			mMatchLock.LeaveReadLock();

			/// match success!
			if (player1 && player2)
			{
				if (!player1->IsConnected())
				{
					mMatchLock.EnterWriteLock();
					mMatchMap.erase(player1->GetPlayerName());
					mMatchLock.LeaveWriteLock();
					continue;
				}

				if (!player2->IsConnected())
				{
					mMatchLock.EnterWriteLock();
					mMatchMap.erase(player2->GetPlayerName());
					mMatchLock.LeaveWriteLock();
					continue;
				}

				/// first, search availabile game sessions
				Aws::GameLift::Model::SearchGameSessionsRequest searchReq;
				searchReq.SetAliasId(mAliasId);
				searchReq.SetFilterExpression("playerSessionCount=0 AND hasAvailablePlayerSessions=true");
				auto searchResult = mGLClient->SearchGameSessions(searchReq);
				if (searchResult.IsSuccess())
				{
					auto availableGs = searchResult.GetResult().GetGameSessions();
					if (availableGs.size() > 0)
					{
						auto ipAddress = availableGs[0].GetIpAddress();
						auto port = availableGs[0].GetPort();
						auto gameSessionId = availableGs[0].GetGameSessionId();

						std::cout << "CreatePlayerSessions on Searched Game Session\n";
						CreatePlayerSessions(player1, player2, ipAddress, gameSessionId, port);
					}
					else
					{
						/// when no available game sessions...
						Aws::GameLift::Model::CreateGameSessionRequest req;
						req.SetAliasId(mAliasId);
						req.SetMaximumPlayerSessionCount(MAX_PLAYER_PER_GAME);

						auto outcome = mGLClient->CreateGameSession(req);
						if (outcome.IsSuccess())
						{
							auto gs = outcome.GetResult().GetGameSession();
							auto port = gs.GetPort();
							auto ipAddress = gs.GetIpAddress();
							auto gameSessionId = gs.GetGameSessionId();

							std::cout << "CreatePlayerSessions on Created Game Session\n";
							CreatePlayerSessions(player1, player2, ipAddress, gameSessionId, port);

						}
						else
						{
							GConsoleLog->PrintOut(true, "CreateGameSession error: %s\n", outcome.GetError().GetExceptionName().c_str());
						}
					}
				}
				else
				{
					GConsoleLog->PrintOut(true, "SearchGameSessions error: %s\n", searchResult.GetError().GetExceptionName().c_str());
				}


				/// remove players from the match queue
				mMatchLock.EnterWriteLock();
				mMatchMap.erase(player1->GetPlayerName());
				mMatchMap.erase(player2->GetPlayerName());
				mMatchLock.LeaveWriteLock();
			}
				
			
			Sleep(100);
		}
	});

	matchThread.detach();
}

void MatchMaker::CreatePlayerSessions(std::shared_ptr<PlayerSession> player1, std::shared_ptr<PlayerSession> player2, const std::string& ipAddress, const std::string& gsId, int port)
{
	Aws::GameLift::Model::CreatePlayerSessionsRequest req;
	req.SetGameSessionId(gsId);
	std::vector<std::string> playerNames;
	playerNames.push_back(player1->GetPlayerName());
	playerNames.push_back(player2->GetPlayerName());
	req.SetPlayerIds(playerNames);

	std::map<std::string, std::string> scoreMap;
	scoreMap[player1->GetPlayerName()] = std::to_string(player1->GetPlayerScore());
	scoreMap[player2->GetPlayerName()] = std::to_string(player2->GetPlayerScore());
	req.SetPlayerDataMap(scoreMap);

	auto result = mGLClient->CreatePlayerSessions(req);
	if (result.IsSuccess())
	{
		auto playerSessionList = result.GetResult().GetPlayerSessions();
		auto player1SessionId = playerSessionList[0].GetPlayerSessionId();
		auto player2SessionId = playerSessionList[1].GetPlayerSessionId();

		player1->SendMatchInfo(port, ipAddress, player1SessionId);
		player2->SendMatchInfo(port, ipAddress, player2SessionId);
	}
	else
	{
		GConsoleLog->PrintOut(true, "CreatePlayerSessions error: %s\n", result.GetError().GetExceptionName().c_str());
	
		/// client should handle in case of "error"
		player1->SendMatchInfo(port, ipAddress, std::string("psess-error"));
		player2->SendMatchInfo(port, ipAddress, std::string("psess-error"));
	}
}


bool MatchMaker::PlayerLogin(std::shared_ptr<PlayerSession> psess, const std::string& playerName, const std::string& playerPass)
{
	if (playerName.size() < 3 || playerPass.size() < 3)
		return false;

	/// password hashing first
	std::size_t passwd_hash = std::hash<std::string>{}(playerPass);

	GetItemRequest getItemRequest;
	getItemRequest.SetTableName(mPlayerTableName);
	Aws::StringStream ss;
	ss << playerName;
	AttributeValue hashKey;
	hashKey.SetS(ss.str());
	getItemRequest.AddKey("PlayerName", hashKey);
	ss.str("");

	auto result = mDDBClient->GetItem(getItemRequest);
	if (!result.IsSuccess())
	{
		GConsoleLog->PrintOut(true, "GetItemRequest error: %s\n", result.GetError().GetMessageA().c_str());
		return false;
	}
		
	auto resultVal = result.GetResult().GetItem();

	/// in case of first login, create a new player
	if (0 == resultVal.size())
	{
		/// put request
		PutItemRequest putItemRequest;
		putItemRequest.SetTableName("GomokuPlayerInfo");
		AttributeValue hashKeyAttribute;
		ss << playerName;
		hashKeyAttribute.SetS(ss.str());
		putItemRequest.AddItem("PlayerName", hashKeyAttribute);
		ss.str("");

		AttributeValue passwdValue;
		ss << passwd_hash;
		passwdValue.SetS(ss.str());
		putItemRequest.AddItem("Password", passwdValue);
		ss.str("");

		AttributeValue scoreValue;
		scoreValue.SetN("1000");
		putItemRequest.AddItem("Score", scoreValue);
		AttributeValue winValue;
		winValue.SetN("0");
		putItemRequest.AddItem("Win", winValue);
		AttributeValue loseValue;
		loseValue.SetN("0");
		putItemRequest.AddItem("Lose", loseValue);

		auto putResult = mDDBClient->PutItem(putItemRequest);

		if (!putResult.IsSuccess())
		{
			GConsoleLog->PrintOut(true, "PutItemRequest error: %s\n", putResult.GetError().GetMessageA().c_str());
			return false;
		}

		psess->SetPlayerInfo(playerName, 1000);
	}
	else
	{
		ss << passwd_hash;

		if (resultVal["Password"].GetS() != ss.str())
		{
			GConsoleLog->PrintOut(true, "Password Mismatch: %s\n", playerName.c_str());
			return false;
		}

		int score = stoi(resultVal["Score"].GetN());
		psess->SetPlayerInfo(playerName, score);
	}

	return true;
}

