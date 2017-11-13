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

    bool IsValid() { return mPlayerSessionId.length() > 0; }

    void PlayerReady(const std::string& playerId);
    void PlayerExit(const std::string& playerId);

    virtual void OnRead(size_t len);
    virtual void OnDisconnect(DisconnectReason dr);

    const std::string& GetPlayerSessionId() { return mPlayerSessionId; }
    const std::string& GetPlayerName() { return mPlayerName; }
    int GetPlayerScore() const { return mScore; }

private:

    std::string mPlayerSessionId;
    std::string mPlayerName;
    int mScore;

};


