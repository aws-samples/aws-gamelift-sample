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
#include "NetController.h"
#include "SharedStruct.h"
#include "GUIController.h"
#include "MatchController.h"

#pragma comment(lib,"ws2_32.lib")

std::unique_ptr<NetController> GGameServer;

NetController::NetController() : mRecvBuffer(BUF_SIZE), mSocket(NULL), mPortNum(-1), mConnected(false)
{
}

NetController::~NetController()
{
    closesocket(mSocket);
}


bool NetController::Connect(const std::string& serverAddr, int port)
{
    if (mConnected)
        return false;

    mServerAddr = serverAddr;
    mPortNum = port;
    mRecvBuffer.BufferReset();

    mSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (mSocket == INVALID_SOCKET)
        return false;

    struct hostent* host;
    struct sockaddr_in hostAddr;

    if ((host = gethostbyname(mServerAddr.c_str())) == 0)
        return false;

    memset(&hostAddr, 0, sizeof(hostAddr));
    hostAddr.sin_family = AF_INET;
    hostAddr.sin_addr.s_addr = ((struct in_addr *)(host->h_addr_list[0]))->s_addr;
    hostAddr.sin_port = htons(static_cast<u_short>(mPortNum));

    if (SOCKET_ERROR == ::connect(mSocket, (struct sockaddr*)&hostAddr, sizeof(hostAddr)))
    {
        std::cout << "CONNECT FAILED\n";
        return false;
    }

    //u_long arg = 1;
    //ioctlsocket(mSocket, FIONBIO, &arg);

    /// nagle off
    int opt = 1;
    setsockopt(mSocket, IPPROTO_TCP, TCP_NODELAY, (const char*)&opt, sizeof(int));

    mConnected = true;

    /// receive thread start
    std::thread(std::bind(&NetController::NetworkThread, this)).detach();


    return true;
}

void NetController::Disconnect()
{
    mConnected = false;
    mPlayerId.clear();

    if (SOCKET_ERROR == ::shutdown(mSocket, SD_BOTH))
    {
        std::cout << "SOCKET SHUTDOWN FAILED " << WSAGetLastError() << std::endl;
    }

    GMatchMaker->ResetMatch();
}


bool NetController::Send(const char* data, int length)
{
    int count = 0;
    while (count < length)
    {
        int n = ::send(mSocket, data + count, length, 0);
        if (n == SOCKET_ERROR)
        {
            std::cout << "SEND ERROR\n";
            return false;
        }
        count += n;
        length -= n;
    }

    return true;
}

void NetController::NetworkThread()
{
    while (mConnected)
    {
        char inBuf[4096] = { 0, };

        int n = ::recv(mSocket, inBuf, 4096, 0);

        if (n < 1)
        {
            Sleep(0); ///< for cpu low-utilization
            continue;
        }


        if (!mRecvBuffer.Write(inBuf, n))
        {
            assert(false);
        }

        ProcessPacket();
    }
}

void NetController::ProcessPacket()
{

    while (true)
    {
        PacketHeader header;

        if (false == mRecvBuffer.Peek((char*)&header, sizeof(PacketHeader)))
            break;


        if (header.mSize > mRecvBuffer.GetStoredSize())
            break;


        switch (header.mType)
        {

        case PKT_SC_START:
        {
            GameStartBroadcast recvData;
            bool ret = mRecvBuffer.Read((char*)&recvData, recvData.mSize);
            assert(ret);

            if (mPlayerId == std::string(recvData.mFirstPlayerId))
            {

                GGuiController->OnGameStart(StoneType::STONE_BLACK, std::string(recvData.mOpponentName));
            }
            else
            {
                GGuiController->OnGameStart(StoneType::STONE_WHITE, std::string(recvData.mOpponentName));
            }
        }
        break;

        case PKT_SC_BOARD_STATUS:
        {
            BoardStatusBroadcast recvData;
            bool ret = mRecvBuffer.Read((char*)&recvData, recvData.mSize);
            assert(ret);

            GGuiController->OnStatusChange(recvData);
        }
        break;

        default:
            assert(false);
        }

    }

}

void NetController::RequestGameStart(const std::string& playerId)
{
    if (!mConnected)
        return;

    mPlayerId = playerId;

    StartRequest sendData;
    sprintf_s(sendData.mPlayerId, "%s", mPlayerId.c_str());

    Send((const char*)&sendData, sizeof(StartRequest));
}

void NetController::RequestPutStone(int x, int y)
{
    if (!mConnected)
        return;

    if (mPlayerId.length() == 0)
        return;

    PutStoneRequest sendData;
    sendData.mPosX = x;
    sendData.mPosY = y;

    Send((const char*)&sendData, sizeof(PutStoneRequest));
}

void NetController::RequestGiveUp()
{
    if (!mConnected)
        return;

    if (mPlayerId.length() == 0)
        return;

    ExitRequest sendData;
    sprintf_s(sendData.mPlayerId, "%s", mPlayerId.c_str());

    Send((const char*)&sendData, sizeof(ExitRequest));
}
