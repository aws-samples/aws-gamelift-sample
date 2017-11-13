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
#include "GUIController.h"
#include "NetController.h"
#include "MatchController.h"
#include "INIReader.h"


int main(int argc, char* argv[])
{
    WSADATA wsa;
    if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
        return -1;

    INIReader iniReader("config.ini");
    if (iniReader.ParseError() < 0)
    {
        std::cout << "config.ini not found\n";
        return 0;
    }

    std::string matchApiEndpoint = iniReader.Get("config", "MATCH_SERVER_API", "https....");
    std::string playerName = iniReader.Get("config", "PLAYER_NAME", "DefaultName");
    std::string playerPass = iniReader.Get("config", "PLAYER_PASSWD", "DefaultPass");

    /// override for test
    if (argc >= 3)
    {
        playerName = std::string(argv[1]);
        playerPass = std::string(argv[2]);
    }

    GGuiController.reset(new GUIController(playerName.c_str(), playerPass.c_str()));
    GMatchMaker.reset(new MatchController(matchApiEndpoint));
    GGameServer.reset(new NetController);

    GGuiController->Initialize(&argc, argv);
    GGuiController->DoEventLoop();

    WSACleanup();
    return 0;
}

