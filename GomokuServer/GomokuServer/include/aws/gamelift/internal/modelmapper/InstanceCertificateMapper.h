/*
* All or portions of this file Copyright(c) Amazon.com, Inc. or its affiliates or
* its licensors.
*
* For complete copyright and license terms please see the LICENSE at the root of this
* distribution(the "License").All use of this software is governed by the License,
* or , if provided, by the license below or the license accompanying this file.Do not
* remove or modify any license notices.This file is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
*
*/
#pragma once
#include <aws/gamelift/server/model/GetInstanceCertificateResult.h>
#include <aws/gamelift/server/protocols/sdk.pb.h>

namespace Aws
{
namespace GameLift
{
namespace Internal
{
    namespace ModelMapper
    {
        using namespace Aws::GameLift::Server::Model;
        using namespace com::amazon::whitewater::auxproxy;

        class InstanceCertificateMapper
        {
        public:
            static void ParseFromBufferedGetInstanceCertificateResponse(const pbuffer::GetInstanceCertificateResponse& getInstanceCertificateResponse, GetInstanceCertificateResult& result)
            {
                result.SetCertificatePath(getInstanceCertificateResponse.certificatepath().c_str());
                result.SetCertificateChainPath(getInstanceCertificateResponse.certificatechainpath().c_str());
                result.SetPrivateKeyPath(getInstanceCertificateResponse.privatekeypath().c_str());
                result.SetHostName(getInstanceCertificateResponse.hostname().c_str());
                result.SetRootCertificatePath(getInstanceCertificateResponse.rootcertificatepath().c_str());
            }
        };
    } //namespace ModelMapper
} //namespace Internal
} //namespace GameLift
} //namespace Aws