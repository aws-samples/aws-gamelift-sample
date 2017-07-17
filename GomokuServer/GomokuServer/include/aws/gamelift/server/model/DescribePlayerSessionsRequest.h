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

#ifndef GAMELIFT_USE_STD
    #ifndef MAX_GAME_SESSION_ID_LENGTH
        #define MAX_GAME_SESSION_ID_LENGTH 256
    #endif
    #ifndef MAX_PLAYER_ID_LENGTH
        #define MAX_PLAYER_ID_LENGTH 1024
    #endif
    #ifndef MAX_PLAYER_SESSION_ID_LENGTH
        #define MAX_PLAYER_SESSION_ID_LENGTH 256
    #endif
    #ifndef MAX_PLAYER_SESSION_STATUS_FILTER_LENGTH
        #define MAX_PLAYER_SESSION_STATUS_FILTER_LENGTH 1024
    #endif
    #ifndef MAX_NEXT_TOKEN_LENGTH
        #define MAX_NEXT_TOKEN_LENGTH 1024
    #endif
#endif

namespace Aws
{
namespace GameLift
{
namespace Server
{
namespace Model
{

  /**
   * <p>Represents the input for a request action.</p>
   */
  class AWS_GAMELIFT_API DescribePlayerSessionsRequest
  {
#ifdef GAMELIFT_USE_STD
  public:
    DescribePlayerSessionsRequest() :
      m_limit(0)
    { }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline const std::string& GetGameSessionId() const{ return m_gameSessionId; }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline void SetGameSessionId(const std::string& value) { m_gameSessionId = value; }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline void SetGameSessionId(std::string&& value) { m_gameSessionId = value; }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline void SetGameSessionId(const char* value) { m_gameSessionId.assign(value); }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline DescribePlayerSessionsRequest& WithGameSessionId(const std::string& value) { SetGameSessionId(value); return *this;}

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline DescribePlayerSessionsRequest& WithGameSessionId(std::string&& value) { SetGameSessionId(value); return *this;}

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline DescribePlayerSessionsRequest& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline const std::string& GetPlayerId() const{ return m_playerId; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(const std::string& value) { m_playerId = value; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(std::string&& value) { m_playerId = value; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(const char* value) { m_playerId.assign(value); }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerId(const std::string& value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerId(std::string&& value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerId(const char* value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline const std::string& GetPlayerSessionId() const{ return m_playerSessionId; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(const std::string& value) { m_playerSessionId = value; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(std::string&& value) { m_playerSessionId = value; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(const char* value) { m_playerSessionId.assign(value); }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionId(const std::string& value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionId(std::string&& value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionId(const char* value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline const std::string& GetPlayerSessionStatusFilter() const{ return m_playerSessionStatusFilter; }

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetPlayerSessionStatusFilter(const std::string& value) { m_playerSessionStatusFilter = value; }

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetPlayerSessionStatusFilter(std::string&& value) { m_playerSessionStatusFilter = value; }

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetPlayerSessionStatusFilter(const char* value) { m_playerSessionStatusFilter.assign(value); }

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionStatusFilter(const std::string& value) { SetPlayerSessionStatusFilter(value); return *this;}

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionStatusFilter(std::string&& value) { SetPlayerSessionStatusFilter(value); return *this;}

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionStatusFilter(const char* value) { SetPlayerSessionStatusFilter(value); return *this;}

    /**
     * <p>Maximum number of results to return. Use this parameter with
     * <code>NextToken</code> to get results as a set of sequential pages. If a player
     * session ID is specified, this parameter is ignored.</p>
     */
    inline int GetLimit() const{ return m_limit; }

    /**
     * <p>Maximum number of results to return. Use this parameter with
     * <code>NextToken</code> to get results as a set of sequential pages. If a player
     * session ID is specified, this parameter is ignored.</p>
     */
    inline void SetLimit(int value) { m_limit = value; }

    /**
     * <p>Maximum number of results to return. Use this parameter with
     * <code>NextToken</code> to get results as a set of sequential pages. If a player
     * session ID is specified, this parameter is ignored.</p>
     */
    inline DescribePlayerSessionsRequest& WithLimit(int value) { SetLimit(value); return *this;}

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline const std::string& GetNextToken() const{ return m_nextToken; }

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline void SetNextToken(const std::string& value) { m_nextToken = value; }

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline void SetNextToken(std::string&& value) { m_nextToken = value; }

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline void SetNextToken(const char* value) { m_nextToken.assign(value); }

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline DescribePlayerSessionsRequest& WithNextToken(const std::string& value) { SetNextToken(value); return *this;}

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline DescribePlayerSessionsRequest& WithNextToken(std::string&& value) { SetNextToken(value); return *this;}

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline DescribePlayerSessionsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:
    std::string m_gameSessionId;
    std::string m_playerId;
    std::string m_playerSessionId;
    std::string m_playerSessionStatusFilter;
    int m_limit;
    std::string m_nextToken;
#else
  public:
    DescribePlayerSessionsRequest() :
      m_limit(0)
    {
      m_gameSessionId[0] = '\0';
      m_playerId[0] = '\0';
      m_playerSessionId[0] = '\0';
      m_playerSessionStatusFilter[0] = '\0';
      m_nextToken[0] = '\0';
    }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline const char* GetGameSessionId() const{ return m_gameSessionId; }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline void SetGameSessionId(const char* value) { strcpy(m_gameSessionId, value); }

    /**
     * <p>Unique identifier for the game session to get player sessions for.Game
     * session ID format is as follows:
     * "arn:aws:gamelift:&lt;region&gt;::gamesession/fleet-&lt;fleet ID&gt;/&lt;ID
     * string&gt;". The value of &lt;ID string&gt; is either a custom ID string (if one
     * was specified when the game session was created) an auto-generated string. </p>
     */
    inline DescribePlayerSessionsRequest& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this;}

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline const char* GetPlayerId() const{ return m_playerId; }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline void SetPlayerId(const char* value) { strcpy(m_playerId, value); }

    /**
     * <p>Unique identifier for a player.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerId(const char* value) { SetPlayerId(value); return *this;}

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline const char* GetPlayerSessionId() const{ return m_playerSessionId; }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline void SetPlayerSessionId(const char* value) { strcpy(m_playerSessionId, value); }

    /**
     * <p>Unique identifier for a player session.</p>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionId(const char* value) { SetPlayerSessionId(value); return *this;}

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline const char* GetPlayerSessionStatusFilter() const{ return m_playerSessionStatusFilter; }

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline void SetPlayerSessionStatusFilter(const char* value) { strcpy(m_playerSessionStatusFilter, value); }

    /**
     * <p>Player session status to filter results on.</p> <p>Possible player session
     * statuses include the following:</p> <ul> <li> <p> <b>RESERVED</b> – The player
     * session request has been received, but the player has not yet connected to the
     * server process and/or been validated. </p> </li> <li> <p> <b>ACTIVE</b> – The
     * player has been validated by the server process and is currently connected.</p>
     * </li> <li> <p> <b>COMPLETED</b> – The player connection has been dropped.</p>
     * </li> <li> <p> <b>TIMEDOUT</b> – A player session request was received, but the
     * player did not connect and/or was not validated within the time-out limit (60
     * seconds).</p> </li> </ul>
     */
    inline DescribePlayerSessionsRequest& WithPlayerSessionStatusFilter(const char* value) { SetPlayerSessionStatusFilter(value); return *this;}

    /**
     * <p>Maximum number of results to return. Use this parameter with
     * <code>NextToken</code> to get results as a set of sequential pages. If a player
     * session ID is specified, this parameter is ignored.</p>
     */
    inline int GetLimit() const{ return m_limit; }

    /**
     * <p>Maximum number of results to return. Use this parameter with
     * <code>NextToken</code> to get results as a set of sequential pages. If a player
     * session ID is specified, this parameter is ignored.</p>
     */
    inline void SetLimit(int value) { m_limit = value; }

    /**
     * <p>Maximum number of results to return. Use this parameter with
     * <code>NextToken</code> to get results as a set of sequential pages. If a player
     * session ID is specified, this parameter is ignored.</p>
     */
    inline DescribePlayerSessionsRequest& WithLimit(int value) { SetLimit(value); return *this;}

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline const char* GetNextToken() const{ return m_nextToken; }

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline void SetNextToken(const char* value) { strcpy(m_nextToken, value); }

    /**
     * <p>Token indicating the start of the next sequential page of results. Use the
     * token that is returned with a previous call to this action. To specify the start
     * of the result set, do not specify a value. If a player session ID is specified,
     * this parameter is ignored.</p>
     */
    inline DescribePlayerSessionsRequest& WithNextToken(const char* value) { SetNextToken(value); return *this;}

  private:
    char m_gameSessionId[MAX_GAME_SESSION_ID_LENGTH];
    char m_playerId[MAX_PLAYER_ID_LENGTH];
    char m_playerSessionId[MAX_PLAYER_SESSION_ID_LENGTH];
    char m_playerSessionStatusFilter[MAX_PLAYER_SESSION_STATUS_FILTER_LENGTH];
    int m_limit;
    char m_nextToken[MAX_NEXT_TOKEN_LENGTH];
#endif
  };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
