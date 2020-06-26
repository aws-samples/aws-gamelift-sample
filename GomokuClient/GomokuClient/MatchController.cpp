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
#include "MatchController.h"
#include "GUIController.h"
#include "NetController.h"

#include <cpprest/http_client.h>
#include <cpprest/json.h>

#include <locale> 
#include <codecvt>

using namespace web;
using namespace web::http;
using namespace web::http::client;

std::unique_ptr<MatchController> GMatchMaker;

std::wstring s2ws(const std::string& str)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.from_bytes(str);
}

std::string ws2s(const std::wstring& wstr)
{
    std::wstring_convert<std::codecvt_utf8<wchar_t>, wchar_t> converter;
    return converter.to_bytes(wstr);
}


void MatchController::RequestMatch()
{
    if (mMatchStarted)
        return;

    json::value postData;

    postData[L"PlayerName"] = json::value::string(s2ws(GGuiController->GetPlayerName()));
    postData[L"PlayerPass"] = json::value::string(s2ws(GGuiController->GetPlayerPassword()));

    auto reqEndpoint = s2ws(mMatchApiEndpoint) + std::wstring(L"/matchrequest");
    http_client client(reqEndpoint);


    client.request(methods::POST, L"", postData.serialize().c_str(), L"application/json").then([this](http_response response) {

        if (response.status_code() == status_codes::OK)
        {
            json::value jsonValue = response.extract_json().get();

            mMatchStarted = true;

            mMatchTicketId = ws2s(jsonValue[L"TicketId"].serialize());

            mMatchTicketId.erase(0, 1);
            mMatchTicketId.erase(mMatchTicketId.size() - 1);

            GGuiController->OnMatchWait(true);

            return true;
        }

        GGuiController->OnMatchWait(false);

        return false;

    }).then([this](bool isOk) {
        if (isOk)
        {
            while (true)
            {
                Sleep(1000);
                if (CheckMatchStatus())
                {
                    GGuiController->OnMatchComplete();
                    break;
                }
            }
        }

    });

}

bool MatchController::CheckMatchStatus()
{  
    bool completed = false;
   
    json::value postData;

    postData[L"PlayerName"] = json::value::string(s2ws(GGuiController->GetPlayerName()));
    postData[L"TicketId"] = json::value::string(s2ws(mMatchTicketId));

    auto reqEndpoint = s2ws(mMatchApiEndpoint) + std::wstring(L"/matchstatus");
    http_client client(reqEndpoint);
    client.request(methods::POST, L"", postData.serialize().c_str(), L"application/json").then([this, &completed](http_response response) {
        
        if (response.status_code() == status_codes::OK)
        {
            json::value jsonValue = response.extract_json().get();

            std::string ipAddr = ws2s(jsonValue[L"IpAddress"].as_string());
            std::string psessId = ws2s(jsonValue[L"PlayerSessionId"].as_string());
            int port = jsonValue[L"Port"].as_integer();

            if (port > 0)
            {
                completed = true;
                if (GGameServer->Connect(ipAddr, port))
                {
                    GGameServer->RequestGameStart(psessId);
                }
                else
                {
                    std::cout << "GameServer Connect Error\n";
                }
            }
        }
    }).wait();

    return completed;
}

void MatchController::ResetMatch()
{
    if (!mMatchStarted)
        return;

    mMatchStarted = false;
    mMatchTicketId.clear();

}