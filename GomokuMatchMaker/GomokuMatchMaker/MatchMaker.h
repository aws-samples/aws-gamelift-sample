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

#include <aws/gamelift/GameLiftClient.h>
#include <aws/dynamodb/DynamoDBClient.h>

#include "FastSpinlock.h"

class PlayerSession;
class MatchMaker
{
public:
	MatchMaker(const std::string& alias, const std::string& playerTable);

	void SetUpAwsClient(const std::string& region);

	bool RequestMatch(std::shared_ptr<PlayerSession> psess);
	void DoMatchMaking();

	bool PlayerLogin(std::shared_ptr<PlayerSession> psess,
		const std::string& playerName,
		const std::string& playerPass);

private:
	void CreatePlayerSessions(std::shared_ptr<PlayerSession> player1, std::shared_ptr<PlayerSession> player2,
		const std::string& ipAddress, const std::string& gsId, int port);

private:
	std::shared_ptr<Aws::GameLift::GameLiftClient> mGLClient;
	std::shared_ptr<Aws::DynamoDB::DynamoDBClient> mDDBClient;

	std::string mAliasId;
	std::string mPlayerTableName;

	FastSpinlock mMatchLock;
	std::map<std::string, std::shared_ptr<PlayerSession>> mMatchMap;
};

extern std::unique_ptr<MatchMaker> GMatchMaker;