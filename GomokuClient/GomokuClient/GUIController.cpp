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

#define WINDOW_SIZE_W	600
#define WINDOW_SIZE_H	600
#define GRID_GAP 30
#define STONE_RADIUS 15.0

std::unique_ptr<GUIController> GGuiController;

void OnWindowResize(int w, int h)
{
    GGuiController->OnWindowResize(w, h);
}

void OnMouseEvent(int button, int state, int x, int y)
{
    GGuiController->OnMouseEvent(button, state, x, y);
}

void OnMenuEvent(int menuId)
{
    GGuiController->OnMenuEvent(menuId);
}

void OnRenderBoard()
{
    GGuiController->OnRenderBoard();
}



GUIController::GUIController(const char* playerName, const char* playerPass)
    : mMyStoneColor(StoneType::STONE_NONE), mCurrentTurn(StoneType::STONE_NONE), mGameStatus(GameStatus::GS_NOT_STARTED)
{
    strcpy_s(mOpponentName, " ");
    strcpy_s(mPlayerName, playerName);
    strcpy_s(mPlayerPass, playerPass);
    memset(mTextLine, 0, sizeof(mTextLine));
    memset(mViewMatrix, 0, sizeof(mViewMatrix));
}


void GUIController::Initialize(int* argcp, char** argv)
{
    glutInit(argcp, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(WINDOW_SIZE_W, WINDOW_SIZE_H);
    glutInitWindowPosition(100, 50);
    glutCreateWindow("Gomoku Client");

    glViewport(0, 0, WINDOW_SIZE_W, WINDOW_SIZE_H);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();
    glOrtho(0.0, (GLdouble)WINDOW_SIZE_W, 0.0, (GLdouble)WINDOW_SIZE_H, -1.0, 1.0);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glFlush();

    glutReshapeFunc(::OnWindowResize);
    glutMouseFunc(::OnMouseEvent);
    glutDisplayFunc(::OnRenderBoard);

    glutCreateMenu(::OnMenuEvent);
    glutAddMenuEntry("GIVEUP", 1);
    glutAddMenuEntry("START", 2);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    SetTextLine("RIGHT MOUSE CLICK and START!");
}



void GUIController::OnWindowResize(int w, int h)
{
    glViewport(0, 0, WINDOW_SIZE_W, WINDOW_SIZE_H);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0.0, (GLdouble)WINDOW_SIZE_W, 0.0, (GLdouble)WINDOW_SIZE_H, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glutReshapeWindow(WINDOW_SIZE_W, WINDOW_SIZE_H);
}

void GUIController::OnMouseEvent(int button, int state, int x, int y)
{
    if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
    {
        if (GRID_GAP / 2 < x  && x < WINDOW_SIZE_W - GRID_GAP / 2 && GRID_GAP / 2 < y && y < WINDOW_SIZE_H - GRID_GAP / 2)
        {
            int adjLocX = x / GRID_GAP;
            int adjLocY = y / GRID_GAP;

            int adjSubX = x % GRID_GAP;
            int adjSubY = y % GRID_GAP;

            if (adjSubX >= GRID_GAP / 2)
                adjLocX += 1;
            if (adjSubY >= GRID_GAP / 2)
                adjLocY += 1;

            if (adjLocX > BOARD_SIZE || adjLocY > BOARD_SIZE)
                return;

            PutStone(adjLocX, adjLocY);
            glutPostRedisplay();
        }
    }
}


void GUIController::OnMenuEvent(int menuId)
{
    if (1 == menuId)
    {
        if (mGameStatus == GameStatus::GS_STARTED)
        {
            GGameServer->RequestGiveUp();
        }
    }
    else if (2 == menuId)
    {
        if (mGameStatus == GameStatus::GS_NOT_STARTED)
        {
            GMatchMaker->RequestMatch();
        }
        else if (mGameStatus == GameStatus::GS_GAME_OVER_BLACK_WIN || mGameStatus == GameStatus::GS_GAME_OVER_WHITE_WIN)
        {
            ResetStatus();
            GMatchMaker->RequestMatch();
        }
    }


    glutPostRedisplay();
}

void GUIController::OnRenderBoard()
{
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glColor3f(0.4f, 0.3f, 0.2f);
    glBegin(GL_POLYGON);
    glVertex2f(GRID_GAP / 4, GRID_GAP / 4);
    glVertex2f(WINDOW_SIZE_W - GRID_GAP / 4, GRID_GAP / 4);
    glVertex2f(WINDOW_SIZE_W - GRID_GAP / 4, WINDOW_SIZE_H - GRID_GAP / 4);
    glVertex2f(GRID_GAP / 4, WINDOW_SIZE_H - GRID_GAP / 4);
    glEnd();

    glColor3f(0, 0, 0);
    glLineWidth(3);
    glBegin(GL_LINE_LOOP);
    glVertex2f(GRID_GAP, GRID_GAP);
    glVertex2f(WINDOW_SIZE_W - GRID_GAP, GRID_GAP);
    glVertex2f(WINDOW_SIZE_W - GRID_GAP, WINDOW_SIZE_H - GRID_GAP);
    glVertex2f(GRID_GAP, WINDOW_SIZE_H - GRID_GAP);
    glEnd();

    for (int x = 0; x <= WINDOW_SIZE_W; x += GRID_GAP)
    {
        glBegin(GL_LINES);
        glVertex2f(x + GRID_GAP, GRID_GAP);
        glVertex2f(x + GRID_GAP, WINDOW_SIZE_H - GRID_GAP);
        glEnd();
    }

    for (int y = 0; y <= WINDOW_SIZE_H; y += GRID_GAP)
    {
        glBegin(GL_LINES);
        glVertex2f(GRID_GAP, y + GRID_GAP);
        glVertex2f(WINDOW_SIZE_W - GRID_GAP, y + GRID_GAP);
        glEnd();
    }

    for (int x = 1; x <= BOARD_SIZE; ++x)
    {
        for (int y = 1; y <= BOARD_SIZE; ++y)
        {
            if (mViewMatrix[x][y] == StoneType::STONE_NONE)
                continue;
            if (mViewMatrix[x][y] == StoneType::STONE_BLACK)
                glColor3f(0, 0, 0);
            else
                glColor3f(1, 1, 1);

            glPushMatrix();
            glBegin(GL_TRIANGLE_FAN);

            for (int i = 0; i < 360; i++)
            {
                const float DEG2RAD = 3.14159f / 180;
                float degInRad = i*DEG2RAD;
                glVertex2f(cos(degInRad)*STONE_RADIUS + (x * GRID_GAP), sin(degInRad)*STONE_RADIUS + (y * GRID_GAP));
            }
            glEnd();
            glPopMatrix();
        }
    }

    /// draw a textline
    glColor3f(1, 1, 0);
    glRasterPos2f(GRID_GAP / 3, GRID_GAP / 3);

    for (int i = 0; i < strlen(mTextLine); ++i)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, mTextLine[i]);
    }

    glutSwapBuffers();
}

void GUIController::ResetStatus()
{
    std::lock_guard<std::mutex> lock(mStatusMutex);

    memset(mViewMatrix, 0, sizeof(mViewMatrix));
    mMyStoneColor = StoneType::STONE_NONE;
    mCurrentTurn = StoneType::STONE_NONE;
    mGameStatus = GameStatus::GS_NOT_STARTED;
}

void GUIController::DoEventLoop()
{
    glutMainLoop();
}

void GUIController::SetTextLine(const std::string& str)
{
    sprintf_s(mTextLine, "[%s vs %s] %s", mPlayerName, mOpponentName, str.c_str());
}

void GUIController::TransViewToData(int& x, int& y)
{
    int newX = 0, newY = 0;

    newX = BOARD_SIZE - y;
    newY = x - 1;
    x = newX;
    y = newY;
}

void GUIController::TransDataToView(int& x, int& y)
{
    int newX = 0, newY = 0;
    newX = y + 1;
    newY = BOARD_SIZE - x;
    x = newX;
    y = newY;
}


void GUIController::PutStone(int x, int y)
{
    if (mGameStatus != GameStatus::GS_STARTED)
    {
        SetTextLine("Game is not ready!");
        return;
    }

    if (mCurrentTurn != mMyStoneColor)
    {
        SetTextLine("Not My Turn");
        return;
    }


    int aX = x;
    int aY = BOARD_SIZE + 1 - y;

    if (mViewMatrix[aX][aY] != StoneType::STONE_NONE)
    {
        SetTextLine("Wrong Position");
        return;
    }

    TransViewToData(aX, aY);
    GGameServer->RequestPutStone(aX, aY);
}

void GUIController::OnMatchWait(bool isOk)
{
    std::lock_guard<std::mutex> lock(mStatusMutex);
    if (isOk)
        SetTextLine("MatchMaking Requested... Please Wait...");
    else
        SetTextLine("MatchMaking Request Failed. Please Check PLAYER_NAME and PLAYER_PASSWD in config.ini file");
}

void GUIController::OnMatchComplete()
{
    std::lock_guard<std::mutex> lock(mStatusMutex);
    SetTextLine("MatchMaking Completed... Starting a Game Soon... Please Wait...");
}

void GUIController::OnGameStart(StoneType myType, const std::string& opponentName)
{
    std::lock_guard<std::mutex> lock(mStatusMutex);

    mMyStoneColor = myType;
    mCurrentTurn = StoneType::STONE_BLACK;
    mGameStatus = GameStatus::GS_STARTED;
    strcpy_s(mOpponentName, opponentName.c_str());

    if (mMyStoneColor == StoneType::STONE_BLACK)
        SetTextLine("Your turn is first (BLACK)");
    else
        SetTextLine("Your turn is second (WHITE)");
}

void GUIController::OnStatusChange(const BoardStatusBroadcast& stat)
{
    std::lock_guard<std::mutex> lock(mStatusMutex);

    mGameStatus = stat.mGameStatus;
    mCurrentTurn = stat.mCurrentTurn;

    for (int i = 0; i < BOARD_SIZE; ++i)
    {
        for (int j = 0; j < BOARD_SIZE; ++j)
        {
            int x = i, y = j;
            TransDataToView(x, y);
            mViewMatrix[x][y] = stat.mBoardStatus.mBoardMatrix[i][j];
        }
    }

    if (mGameStatus == GameStatus::GS_GAME_OVER_BLACK_WIN)
    {
        SetTextLine("Player BLACK Win!");
        GGameServer->Disconnect();
    }

    if (mGameStatus == GameStatus::GS_GAME_OVER_WHITE_WIN)
    {
        SetTextLine("Player WHITE Win!");
        GGameServer->Disconnect();
    }
}
