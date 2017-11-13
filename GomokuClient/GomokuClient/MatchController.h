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

class MatchController
{
public:
    MatchController(const std::string& matchApiEndpoint)
    : mMatchApiEndpoint(matchApiEndpoint), mMatchStarted(false)
    {}

    void RequestMatch();

    bool CheckMatchStatus();

    void ResetMatch();

private:

    const std::string mMatchApiEndpoint;
    
    std::atomic_bool mMatchStarted;
    std::string mMatchTicketId;

};

extern std::unique_ptr<MatchController> GMatchMaker;