/*
* All or portions of this file Copyright (c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution (the "License"). All use of this software is governed by the License,
* or, if provided, by the license below or the license accompanying this file. Do not
* remove or modify any license notices. This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once
#include <aws/gamelift/internal/network/AuxProxyMessageHandler.h>
#include <aws/gamelift/internal/network/AuxProxyMessageSender.h>
#include <aws/gamelift/server/protocols/sdk.pb.h>
#include <sio_client.h>
#include <condition_variable>

#define MAIN_PID "MAIN_PID"

namespace Aws
{
namespace GameLift
{
namespace Internal
{
namespace Network
{
    class Network
    {
    public:
        Network(sio::client* sioClient, AuxProxyMessageHandler* handler, AuxProxyMessageSender* sender);

        ~Network();

        void OnConnected();
        void OnFail();
        void OnClose(sio::client::close_reason const& reason);

        void OnStartGameSession(std::string const& name, sio::message::ptr const& data, bool hasAck, sio::message::list &ack_resp);
        void OnTerminateProcess(std::string const& name, sio::message::ptr const& data, bool hasAck, sio::message::list &ack_resp);

        AuxProxyMessageSender* getAuxProxySender() { return m_sender; };

    private:
        Aws::GameLift::Server::Model::GameSession ParseFromBufferedGameSession(com::amazon::whitewater::auxproxy::pbuffer::GameSession bGameSession);
        sio::client* m_sio_client;
        AuxProxyMessageHandler* m_handler;
        AuxProxyMessageSender* m_sender;

        bool m_connect_finish;
        std::mutex m_lock;
        std::condition_variable_any m_cond;
    };
} //namespace Network
} //namespace Internal
} //namespace GameLift
} //namespace Aws
