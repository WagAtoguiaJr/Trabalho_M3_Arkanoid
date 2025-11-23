#ifndef MENU_H
#define MENU_H

#include "raylib.h"
#include <string>
#include <vector>

// Estados do jogo (compartilhados com main)
enum GameState { MENU, PLAYING, RANKING, CONTROLS, PAUSED, GAMEOVER };

// Inicialização do módulo de menu
void InitMenu();

// Atualiza o menu (navegação e seleção)
void UpdateMenu(GameState &state, int &fases, bool &shouldStartGame);

// Desenha o menu principal
void DrawMenu();

// Desenha tela de controles
void DrawControlsScreen();

// Desenha tela de gameover (simples)
void DrawGameOverScreen(bool vitoria);

// Ajusta dificuldade (0=Fácil,1=Médio,2=Difícil)
void SetMenuDifficulty(int diff);
int GetMenuDifficulty();

#endif // MENU_H