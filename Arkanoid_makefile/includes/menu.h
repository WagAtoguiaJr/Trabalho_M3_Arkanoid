#pragma once

#include "raylib.h"
#include <string>
#include <ctime>

using namespace std;

enum GameState { MENU, PLAYING, RANKING, CONTROLS, PAUSED, GAMEOVER };

void InitMenu();

void UpdateMenu(GameState &state, int &fases, bool &shouldStartGame);

void DrawMenu();

void DrawControlsScreen();

void DrawGameOverScreen(const bool &vitoria, const bool &allowSave, const bool &scoreSaved, const int &score, const string &inputPlayerName);

void SetMenuDifficulty(int diff);

int GetMenuDifficulty();