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
#include "Session.h"


class PlayerSession : public Session
{
public:
	PlayerSession(SOCKET sock);
	virtual ~PlayerSession();

	virtual void OnRead(size_t len);

	void SetPlayerInfo(const std::string& playerName, int score)
	{ 
		mPlayerName = playerName;
		mScore = score;
	}

	const std::string& GetPlayerName() { return mPlayerName; }
	const int GetPlayerScore() { return mScore; }

	void SendMatchInfo(int port, const std::string& ipAddr, const std::string& psessId);

private:

	std::string	mPlayerName;
	int mScore;
};


