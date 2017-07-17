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
#include <aws/core/Aws.h>

#include "Exception.h"
#include "Log.h"
#include "INIReader.h"
#include "IocpManager.h"
#include "MatchMaker.h"

int main()
{
	Aws::SDKOptions options;
	Aws::InitAPI(options);

	LThreadType = THREAD_MAIN;

	/// for dump on crash
	SetUnhandledExceptionFilter(ExceptionFilter);
	

	INIReader iniReader("config.ini");
	if (iniReader.ParseError() < 0)
	{
		std::cout << "config.ini not found\n";
		return 0;
	}

	const std::string& alias_id = iniReader.Get("config", "GAMELIFT_ALIAS", "TEST_LOCAL");
	const std::string& playertable_name = iniReader.Get("config", "PLAYER_TABLENAME", "DynamoDBTableName");
	const std::string& gl_region = iniReader.Get("config", "GAMELIFT_REGION", "ap-northeast-1");
	int port = iniReader.GetInteger("config", "LISTEN_PORT", 5999);

	/// Global Managers
	GConsoleLog.reset(new ConsoleLog(".\\matchMakerLog.txt"));

	GMatchMaker.reset(new MatchMaker(alias_id, playertable_name));
	
	GIocpManager.reset(new IocpManager);

	if (false == GIocpManager->Initialize(static_cast<u_short>(port)))
		return -1;

	GMatchMaker->SetUpAwsClient(gl_region);

	if (false == GIocpManager->StartIoThreads())
		return -1;

	GConsoleLog->PrintOut(true, "Start MatchMaker\n");

	GMatchMaker->DoMatchMaking();

	GIocpManager->StartAccept(); ///< block here...

	GIocpManager->Finalize();

	Aws::ShutdownAPI(options);

	GConsoleLog->PrintOut(true, "End MatchMaker\n");
    return 0;
}

