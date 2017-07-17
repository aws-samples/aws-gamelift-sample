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
#include <aws/gamelift/common/GameLift_EXPORTS.h>

namespace Aws
{
namespace GameLift
{
namespace Server
{
namespace Model
{
  enum class GameSessionStatus
  {
    NOT_SET,
    ACTIVE,
    ACTIVATING,
    TERMINATED,
    TERMINATING
  };

namespace GameSessionStatusMapper
{
#ifdef GAMELIFT_USE_STD
    AWS_GAMELIFT_API GameSessionStatus GetGameSessionStatusForName(const std::string& name);
    AWS_GAMELIFT_API std::string GetNameForGameSessionStatus(GameSessionStatus value);
#else
    AWS_GAMELIFT_API GameSessionStatus GetGameSessionStatusForName(const char* name);
    AWS_GAMELIFT_API const char* GetNameForGameSessionStatus(GameSessionStatus value);
#endif
} // namespace GameSessionStatusMapper
} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
