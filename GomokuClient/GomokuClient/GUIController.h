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

#pragma once
#include "SharedStruct.h"

class GUIController
{
public:
    GUIController(const char* playerName, const char* playerPass);

    void Initialize(int* argcp, char** argv);
    void PutStone(int x, int y);

    void OnWindowResize(int w, int h);
    void OnMouseEvent(int button, int state, int x, int y);
    void OnMenuEvent(int menuId);
    void OnRenderBoard();

    void DoEventLoop();
    void ResetStatus();

    void TransViewToData(int& x, int& y);
    void TransDataToView(int& x, int& y);
    void SetTextLine(const std::string& str);

    void OnMatchWait(bool isOk);
    void OnMatchComplete();
    void OnGameStart(StoneType myType, const std::string& opponentName);
    void OnStatusChange(const BoardStatusBroadcast& stat);

    const char* GetPlayerName() const { return mPlayerName; }
    const char* GetPlayerPassword() const { return mPlayerPass; }


private:
    StoneType mMyStoneColor;
    StoneType mCurrentTurn;
    GameStatus mGameStatus;

    char mOpponentName[MAX_STRING_LEN];

    char mPlayerName[MAX_STRING_LEN];
    char mPlayerPass[MAX_STRING_LEN];
    char mTextLine[128];
    StoneType mViewMatrix[BOARD_SIZE + 1][BOARD_SIZE + 1]; ///< 1~19 x 1~19
    std::mutex mStatusMutex;
};

extern std::unique_ptr<GUIController> GGuiController;