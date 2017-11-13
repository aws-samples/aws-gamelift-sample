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
#include "Exception.h"
#include "Log.h"
#include "Session.h"
#include "IocpManager.h"
#include "GameLiftManager.h"
#include "INIReader.h"

#include <aws/core/Aws.h>

int main(int argc, char* argv[])
{
    Aws::SDKOptions options;
    Aws::InitAPI(options);

    int portNum = 0;
    /// listen port override rather than dynamic port by OS
    if (argc >= 2)
        portNum = atoi(argv[1]);

    LThreadType = THREAD_MAIN;

    /// for dump on crash
    SetUnhandledExceptionFilter(ExceptionFilter);

    INIReader iniReader("config.ini");
    if (iniReader.ParseError() < 0)
    {
        std::cout << "config.ini not found\n";
        return 0;
    }

    GConsoleLog.reset(new ConsoleLog(".\\logs\\serverLog.txt"));

    /// GameLift
    GGameLiftManager.reset(new GameLiftManager);

    const std::string& sqs_endpoint = iniReader.Get("config", "SQS_ENDPOINT", "SQS_ENDPOINT");
    const std::string& sqs_ak = iniReader.Get("config", "SQS_ACCESSKEY", "SQS_ACCESSKEY");
    const std::string& sqs_sk = iniReader.Get("config", "SQS_SECRETKEY", "SQS_SECRETKEY");
    const std::string& sqs_region = iniReader.Get("config", "SQS_REGION", "SQS_REGION");

    GGameLiftManager->SetSQSClientInfo(sqs_region, sqs_endpoint, sqs_ak, sqs_sk);

    /// Global Managers
    GIocpManager.reset(new IocpManager);

    if (false == GIocpManager->Initialize(portNum))
        return -1;

    /// Gamelift init/start!
    if (false == GGameLiftManager->InitializeGameLift(portNum))
        return -1;

    if (false == GIocpManager->StartIoThreads())
        return -1;

    GConsoleLog->PrintOut(true, "Start Server\n");

    GIocpManager->StartAccept(); ///< block here...

    GIocpManager->Finalize();

    /// Gamelift end!
    GGameLiftManager->FinalizeGameLift();

    GConsoleLog->PrintOut(true, "End Server\n");

    Aws::ShutdownAPI(options);

    return 0;
}


