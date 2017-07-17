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
#include <aws/gamelift/server/model/GameSessionStatus.h>
#include <aws/gamelift/server/model/PlayerSessionCreationPolicy.h>
#include <aws/gamelift/server/model/GameProperty.h>


#ifndef GAMELIFT_USE_STD
    #ifndef MAX_GAME_PROPERTIES
        #define MAX_GAME_PROPERTIES 32
    #endif
    #ifndef MAX_IP_LENGTH
        #define MAX_IP_LENGTH 45
    #endif
    #ifndef MAX_SESSION_ID_LENGTH
        #define MAX_SESSION_ID_LENGTH 256
    #endif
    #ifndef MAX_SESSION_NAME_LENGTH
        #define MAX_SESSION_NAME_LENGTH 256
    #endif
    #ifndef MAX_FLEET_ID_LENGTH
        #define MAX_FLEET_ID_LENGTH 1024
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
    * <p>Properties describing a game session.</p>
    */
    class AWS_GAMELIFT_API GameSession
    {
#ifdef GAMELIFT_USE_STD
    public:
        GameSession() :
            m_maximumPlayerSessionCount(0),
            m_port(0)
        { }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline const std::string& GetGameSessionId() const{ return m_gameSessionId; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(const std::string& value) { m_gameSessionId = value; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(std::string&& value) { m_gameSessionId = value; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(const char* value) { m_gameSessionId.assign(value); }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(const std::string& value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(std::string&& value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline const std::string& GetName() const{ return m_name; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(const std::string& value) { m_name = value; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(std::string&& value) { m_name = value; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(const char* value) { m_name.assign(value); }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(const std::string& value) { SetName(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(std::string&& value) { SetName(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(const char* value) { SetName(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline const std::string& GetFleetId() const{ return m_fleetId; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(const std::string& value) { m_fleetId = value; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(std::string&& value) { m_fleetId = value; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(const char* value) { m_fleetId.assign(value); }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(const std::string& value) { SetFleetId(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(std::string&& value) { SetFleetId(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(const char* value) { SetFleetId(value); return *this; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline int GetMaximumPlayerSessionCount() const{ return m_maximumPlayerSessionCount; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline void SetMaximumPlayerSessionCount(int value) { m_maximumPlayerSessionCount = value; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline GameSession& WithMaximumPlayerSessionCount(int value) { SetMaximumPlayerSessionCount(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline const GameSessionStatus& GetStatus() const{ return m_status; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(const GameSessionStatus& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(GameSessionStatus&& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(const GameSessionStatus& value) { SetStatus(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(GameSessionStatus&& value) { SetStatus(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline const std::vector<GameProperty>& GetGameProperties() const{ return m_gameProperties; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline void SetGameProperties(const std::vector<GameProperty>& value) { m_gameProperties = value; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline void SetGameProperties(std::vector<GameProperty>&& value) { m_gameProperties = value; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& WithGameProperties(const std::vector<GameProperty>& value) { SetGameProperties(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& WithGameProperties(std::vector<GameProperty>&& value) { SetGameProperties(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& AddGameProperties(const GameProperty& value) { m_gameProperties.push_back(value); return *this; }

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& AddGameProperties(GameProperty&& value) { m_gameProperties.push_back(value); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline const std::string& GetIpAddress() const{ return m_ipAddress; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(const std::string& value) { m_ipAddress = value; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(std::string&& value) { m_ipAddress = value; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(const char* value) { m_ipAddress.assign(value); }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(const std::string& value) { SetIpAddress(value); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(std::string&& value) { SetIpAddress(value); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(const char* value) { SetIpAddress(value); return *this; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline int GetPort() const{ return m_port; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetPort(int value) { m_port = value; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithPort(int value) { SetPort(value); return *this; }

    private:
        std::string m_gameSessionId;
        std::string m_name;
        std::string m_fleetId;
        int m_maximumPlayerSessionCount;
        GameSessionStatus m_status;
        std::vector<GameProperty> m_gameProperties;
        std::string m_ipAddress;
        int m_port;
#else
    public:
        GameSession() :
            m_maximumPlayerSessionCount(0),
            m_port(0), 
			m_gameProperties_count(0)
        { }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline const char* GetGameSessionId() const{ return m_gameSessionId; }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline void SetGameSessionId(const char* value) { strcpy(m_gameSessionId, value); }

        /**
        * <p>Unique identifier for a game session.</p>
        */
        inline GameSession& WithGameSessionId(const char* value) { SetGameSessionId(value); return *this; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline const char* GetName() const{ return m_name; }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline void SetName(const char* value) { strcpy(m_name, value); }

        /**
        * <p>Descriptive label associated with a game session. Session names do not need
        * to be unique.</p>
        */
        inline GameSession& WithName(const char* value) { SetName(value); return *this; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline const char* GetFleetId() const{ return m_fleetId; }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline void SetFleetId(const char* value) { strcpy(m_fleetId, value); }

        /**
        * <p>Unique identifier for a fleet.</p>
        */
        inline GameSession& WithFleetId(const char* value) { SetFleetId(value); return *this; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline int GetMaximumPlayerSessionCount() const{ return m_maximumPlayerSessionCount; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline void SetMaximumPlayerSessionCount(int value) { m_maximumPlayerSessionCount = value; }

        /**
        * <p>Maximum number of players allowed in the game session.</p>
        */
        inline GameSession& WithMaximumPlayerSessionCount(int value) { SetMaximumPlayerSessionCount(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline const GameSessionStatus& GetStatus() const{ return m_status; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(const GameSessionStatus& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline void SetStatus(GameSessionStatus&& value) { m_status = value; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(const GameSessionStatus& value) { SetStatus(value); return *this; }

        /**
        * <p>Current status of the game session. A game session must be in an
        * <code>ACTIVE</code> state to have player sessions.</p>
        */
        inline GameSession& WithStatus(GameSessionStatus&& value) { SetStatus(value); return *this; }

        /**
        * <p>Get the custom properties for the game session.</p>
        */
        inline const GameProperty* GetGameProperties(int& count) const{ count = m_gameProperties_count; return m_gameProperties; }

        /**
        * <p>Set of custom property for the game session.</p>
        */
        inline void AddGameProperty(GameProperty gameProperty) {
            if (m_gameProperties_count < MAX_GAME_PROPERTIES) {
                m_gameProperties[m_gameProperties_count++] = gameProperty;
            }
        };

        /**
        * <p>Set of custom properties for the game session.</p>
        */
        inline GameSession& WithGameProperty(GameProperty gameProperty) { AddGameProperty(gameProperty); return *this; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline const char* GetIpAddress() const{ return m_ipAddress; }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetIpAddress(const char* value) { strcpy(m_ipAddress, value); }

        /**
        * <p>IP address of the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithIpAddress(const char* value) { SetIpAddress(value); return *this; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline int GetPort() const{ return m_port; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline void SetPort(int value) { m_port = value; }

        /**
        * <p>Port number for the game session. To connect to a GameLift server process, an
        * app needs both the IP address and port number.</p>
        */
        inline GameSession& WithPort(int value) { SetPort(value); return *this; }

    private:
        char m_gameSessionId[MAX_SESSION_ID_LENGTH];
        char m_name[MAX_SESSION_NAME_LENGTH];
        char m_fleetId[MAX_FLEET_ID_LENGTH];
        int m_maximumPlayerSessionCount;
        GameSessionStatus m_status;
        GameProperty m_gameProperties[MAX_GAME_PROPERTIES];
        int m_gameProperties_count;
        char m_ipAddress[MAX_IP_LENGTH];
        int m_port;
#endif
    };

} // namespace Model
} // namespace Server
} // namespace GameLift
} // namespace Aws
