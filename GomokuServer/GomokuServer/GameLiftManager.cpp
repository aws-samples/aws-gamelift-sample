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

#include <aws/gamelift/server/GameLiftServerAPI.h>
#include <aws/core/Aws.h>
#include <aws/core/auth/AWSCredentialsProvider.h>
#include <aws/core/client/ClientConfiguration.h>
#include <aws/sqs/SQSClient.h>
#include <aws/sqs/model/SendMessageBatchRequest.h>
#include <aws/sqs/model/SendMessageBatchResult.h>

#include "Log.h"
#include "SharedStruct.h"
#include "GameLiftManager.h"
#include "GameSession.h"

std::unique_ptr<GameLiftManager> GGameLiftManager(nullptr);

GameLiftManager::GameLiftManager() : mActivated(false), mCheckTerminationCount(0), mPlayerReadyCount(0)
{
}

bool GameLiftManager::InitializeGameLift(int listenPort)
{
	auto initOutcome = Aws::GameLift::Server::InitSDK();

	if (!initOutcome.IsSuccess())
		return false;

	std::string serverOut("logs\\serverOut.log");
	std::vector<std::string> logPaths;
	logPaths.push_back(serverOut);

	auto processReadyParameter = Aws::GameLift::Server::ProcessParameters(
		std::bind(&GameLiftManager::OnStartGameSession, this, std::placeholders::_1),
		std::bind(&GameLiftManager::OnProcessTerminate, this),
		std::bind(&GameLiftManager::OnHealthCheck, this),
		listenPort, Aws::GameLift::Server::LogParameters(logPaths)
		);

	auto readyOutcome = Aws::GameLift::Server::ProcessReady(processReadyParameter);

	if (!readyOutcome.IsSuccess())
		return false;

	mActivated = true;

	GConsoleLog->PrintOut(true, "[GAMELIFT] ProcessReady Success (Listen port:%d)\n", listenPort);

	return true;
}


void GameLiftManager::SetSQSClientInfo(const std::string& region, const std::string& url, const std::string& ak, const std::string& sk)
{
	mSQSUrl = url;
	mSQSRegion = region;
	mSQSAk = ak;
	mSQSSk = sk;
}

void GameLiftManager::SendGameResultToSQS(const std::string& blackJson, const std::string& whiteJson) const
{
	/// Setup SQS Client
	Aws::Client::ClientConfiguration config;
	config.scheme = Aws::Http::Scheme::HTTPS;
	config.connectTimeoutMs = 10000;
	config.requestTimeoutMs = 10000;
	config.region = mSQSRegion;

	Aws::Auth::AWSCredentials cred(mSQSAk, mSQSSk);
	Aws::SQS::SQSClient sqsClient(cred, config);
	
	Aws::SQS::Model::SendMessageBatchRequest smReq;

	smReq.SetQueueUrl(mSQSUrl);
	std::vector<Aws::SQS::Model::SendMessageBatchRequestEntry> items;
	Aws::SQS::Model::SendMessageBatchRequestEntry item1, item2;
	item1.SetId("msg_player_001");
	item1.SetMessageBody(blackJson);
	item2.SetId("msg_player_002");
	item2.SetMessageBody(whiteJson);
	items.push_back(item1);
	items.push_back(item2);
	smReq.SetEntries(items);

	auto result = sqsClient.SendMessageBatch(smReq);
	if (!result.IsSuccess())
	{
		GConsoleLog->PrintOut(true, "SQS SendMessageBatch Error: %s\n", result.GetError().GetMessageA().c_str());
	}
}

void GameLiftManager::FinalizeGameLift()
{
	Aws::GameLift::Server::Destroy();
}

bool GameLiftManager::AcceptPlayerSession(std::shared_ptr<PlayerSession> psess, const std::string& playerSessionId)
{
	FastSpinlockGuard lock(mLock);

	auto outcome = Aws::GameLift::Server::AcceptPlayerSession(playerSessionId);

	if (outcome.IsSuccess())
	{
		mGameSession->PlayerEnter(psess);
		return true;
	}
	
	GConsoleLog->PrintOut(true, "[GAMELIFT] AcceptPlayerSession Fail: %s\n", outcome.GetError().GetErrorMessage().c_str());
	return false;
}

void GameLiftManager::RemovePlayerSession(std::shared_ptr<PlayerSession> psess, const std::string& playerSessionId)
{
	FastSpinlockGuard lock(mLock);

	auto outcome = Aws::GameLift::Server::RemovePlayerSession(playerSessionId);
	if (outcome.IsSuccess())
	{
		mGameSession->PlayerLeave(psess);
	}
	else
	{
		GConsoleLog->PrintOut(true, "[GAMELIFT] RemovePlayerSession Fail: %d %s\n", 
			outcome.GetError().GetErrorType(),
			outcome.GetError().GetErrorName().c_str());
	}

	if (++mCheckTerminationCount < MAX_PLAYER_PER_GAME)
		return;

	if (mGameSession->IsEnd() && mActivated)
	{
		GConsoleLog->PrintOut(true, "[GAMELIFT] Terminate GameSession\n");
		TerminateGameSession(37);
	}

}

void GameLiftManager::OnStartGameSession(Aws::GameLift::Server::Model::GameSession myGameSession)
{
	FastSpinlockGuard lock(mLock);
	Aws::GameLift::Server::ActivateGameSession();

	/// create a game session
	mGameSession = std::make_shared<GameSession>(myGameSession.GetMatchmakerData());

	GConsoleLog->PrintOut(true, "[GAMELIFT] OnStartGameSession Success\n");
}

void GameLiftManager::OnProcessTerminate()
{
	FastSpinlockGuard lock(mLock);

	// game-specific tasks required to gracefully shut down a game session, 
	// such as notifying players, preserving game state data, and other cleanup
	if (mActivated)
	{
		GConsoleLog->PrintOut(true, "[GAMELIFT] OnProcessTerminate Success\n");
		TerminateGameSession(0xDEAD);
	}
	
}

void GameLiftManager::TerminateGameSession(int exitCode)
{
	mGameSession.reset(); ///< explicitly release

	Aws::GameLift::Server::TerminateGameSession();

	Aws::GameLift::Server::ProcessEnding();

	mActivated = false;

	::TerminateProcess(::GetCurrentProcess(), exitCode);
}

void GameLiftManager::CheckReadyAll()
{
	if (MAX_PLAYER_PER_GAME != InterlockedIncrement(&mPlayerReadyCount))
		return;

	mGameSession->BroadcastGameStart();
}