# Gomoku Client — Teammate Setup Guide

## Prerequisites

- Windows PC
- Visual Studio 2019 or 2022 with **Desktop development with C++** workload installed
- Internet access (NuGet + AWS)

---

## Important before you start

1. **Two players required** — FlexMatch pairs exactly 2 players. One player alone waits forever.
2. **Backend must be running** — whoever has AWS access must run `start_all.ps1` and wait ~15 min for GameLift to activate before any match can complete.

---

## Step 1 — Get the client source

Copy this folder to your machine (USB / shared drive / zip):

```
aws-gamelift-sample\GomokuClient\
```

---

## Step 2 — Open in Visual Studio

Open `GomokuClient\GomokuClient.sln`

Visual Studio will prompt to restore NuGet packages — click **Restore**.  
This downloads three packages automatically:
- `cpprestsdk` 2.9.1 — HTTP calls to the matchmaking API
- `freeglut` + `nupengl` — OpenGL game UI

If restore fails: **Tools → NuGet Package Manager → Manage NuGet Packages for Solution → Restore**

---

## Step 3 — Fix the API endpoint paths

Our deployed API uses different paths from the original sample code.  
Open `GomokuClient\GomokuClient\MatchController.cpp` and make **2 changes**:

### Change 1 — Match request path (line ~57)

```cpp
// BEFORE:
auto reqEndpoint = s2ws(mMatchApiEndpoint) + std::wstring(L"/matchrequest");

// AFTER:
auto reqEndpoint = s2ws(mMatchApiEndpoint) + std::wstring(L"/match");
```

### Change 2 — Match status call (lines ~110–114)

```cpp
// BEFORE:
auto reqEndpoint = s2ws(mMatchApiEndpoint) + std::wstring(L"/matchstatus");
http_client client(reqEndpoint);
client.request(methods::POST, L"", postData.serialize().c_str(), L"application/json")

// AFTER:
auto ticketId   = s2ws(mMatchTicketId);
auto playerName = s2ws(GGuiController->GetPlayerName());
auto reqEndpoint = s2ws(mMatchApiEndpoint)
    + std::wstring(L"/match?TicketId=") + ticketId
    + std::wstring(L"&PlayerName=") + playerName;
http_client client(reqEndpoint);
client.request(methods::GET, L"")
```

---

## Step 4 — Set config.ini

Open `GomokuClient\GomokuClient\config.ini`

Each player must use a **different PLAYER_NAME**.

### Scenario A — Lambda + API Gateway

```ini
[config]
MATCH_SERVER_API = https://x8knjlnw63.execute-api.us-east-1.amazonaws.com/v1
PLAYER_NAME      = Player1
PLAYER_PASSWD    = simplepw1
```

### Scenario B — ECS Fargate + ALB (via CloudFront)

```ini
[config]
MATCH_SERVER_API = https://dc1lr3m7ptf6i.cloudfront.net
PLAYER_NAME      = Player1
PLAYER_PASSWD    = simplepw1
```

Player 2 uses `Player2`, Player 3 uses `Player3`, etc. — just change `PLAYER_NAME`.  
Password can be anything, all players can use the same one.

---

## Step 5 — Build

1. Set configuration to **Release** and platform to **x64** (top toolbar)
2. **Build → Build Solution** (Ctrl+Shift+B)
3. Executable appears in `GomokuClient\x64\Release\GomokuClient.exe`

---

## Step 6 — Play

1. Both players launch `GomokuClient.exe`
2. Both click **Match Request** within a few seconds of each other
3. Wait ~30 seconds for FlexMatch to pair you
4. Game board opens — Black moves first

---

## Troubleshooting

| Problem | Cause | Fix |
|---|---|---|
| Waiting forever for match | Only 1 player online | Need a second person to also click Match Request |
| Match found but board never opens | GameLift fleet not running | Run `start_all.ps1`, wait 15 min |
| HTTP error / connection refused | Services stopped | Run `start_all.ps1` first |
| Build error: cannot open cpprestsdk | NuGet restore failed | Right-click solution → Restore NuGet Packages |
| Build error: GLUT not found | nupengl restore failed | Same as above |

---

## Switching between scenarios

To compare Scenario A vs Scenario B, just change the `MATCH_SERVER_API` line in  
`config.ini` and relaunch — no rebuild needed. Both players must use the same scenario URL.
