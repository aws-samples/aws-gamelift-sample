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

            return true;
        }

        std::cout << "MatchRequest Error" << std::endl;
        return false;

    }).then([this](bool isOk) {
        if (isOk)
        {
            Sleep(500);
            while (true)
            {
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
            int port = std::stoi(ws2s(jsonValue[L"Port"].as_string()));

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

            std::wcout << jsonValue.serialize() << std::endl; //{"IpAddress":"NotYet","PlayerSessionId":"NotYet","Port":"0"}
           
        }
    }).wait();

    return completed;
}

