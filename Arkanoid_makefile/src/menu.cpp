#include "menu.h"
#include "audio.h" // chama PlayMenuNavigateSound / PlayMenuSelectSound
#include <iostream>

static int menuIndex = 0;
static const char* menuItems[] = { "Iniciar Jogo", "Dificuldade", "Controles", "Sair" };
static const int MENU_COUNT = 4;
static int difficulty = 0; // 0=Fácil,1=Médio,2=Difícil

void InitMenu() {
    menuIndex = 0;
    difficulty = 1;
}

void SetMenuDifficulty(int diff) {
    difficulty = diff % 3;
    if (difficulty < 0) difficulty += 3;
}

int GetMenuDifficulty() {
    return difficulty;
}

static const char* DifficultyText(int d) {
    return (d == 0) ? "Facil" : (d == 1) ? "Medio" : "Dificil";
}

void UpdateMenu(GameState &state, int &fases, bool &shouldStartGame) {
    // Navegação
    if (IsKeyPressed(KEY_DOWN)) {
        menuIndex = (menuIndex + 1) % MENU_COUNT;
        PlayMenuNavigateSound();
    }
    if (IsKeyPressed(KEY_UP)) {
        menuIndex = (menuIndex - 1 + MENU_COUNT) % MENU_COUNT;
        PlayMenuNavigateSound();
    }

    // Alternar dificuldade quando o item estiver selecionado
    if (menuIndex == 1) {
        if (IsKeyPressed(KEY_LEFT)) {
            SetMenuDifficulty((difficulty - 1 + 3) % 3);
            PlayMenuNavigateSound();
        }
        if (IsKeyPressed(KEY_RIGHT)) {
            SetMenuDifficulty((difficulty + 1) % 3);
            PlayMenuNavigateSound();
        }
    }

    // Seleção
    if (IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)) {
        PlayMenuSelectSound();
        switch (menuIndex) {
            case 0: // Iniciar Jogo
                shouldStartGame = true;
                state = PLAYING;
                break;
            case 1: // Dificuldade (já alterada com left/right)
                // apenas feedback sonoro já tocado
                break;
            case 2: // Controles
                state = CONTROLS;
                break;
            case 3: // Sair
                CloseWindow(); // força saída do loop principal
                break;
        }
    }
}

void DrawMenu() {
    BeginDrawing();
    ClearBackground(BLACK);

    const int titleSize = 48;
    const char* title = "ARKANOID";
    DrawText(title, GetScreenWidth()/2 - MeasureText(title, titleSize)/2, 40, titleSize, RAYWHITE);

    int startY = 140;
    for (int i = 0; i < MENU_COUNT; ++i) {
        Color col = (i == menuIndex) ? YELLOW : WHITE;
        DrawText(menuItems[i], GetScreenWidth()/2 - MeasureText(menuItems[i], 28)/2, startY + i*56, 28, col);

        if (i == 1) {
            DrawText(DifficultyText(difficulty), GetScreenWidth()/2 + 160, startY + i*56, 22, LIGHTGRAY);
        }
    }

    DrawText("Use UP/DOWN para navegar, LEFT/RIGHT para dificuldade, ENTER para selecionar", 10, GetScreenHeight() - 28, 18, LIGHTGRAY);
    EndDrawing();
}

void DrawControlsScreen() {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("CONTROLES", GetScreenWidth()/2 - MeasureText("CONTROLES", 40)/2, 20, 40, RAYWHITE);
    DrawText("Setas Esquerda/Direita - mover paddle", 60, 100, 22, WHITE);
    DrawText("Espaco - soltar bola", 60, 140, 22, WHITE);
    DrawText("P - pausar", 60, 180, 22, WHITE);
    DrawText("B - voltar ao menu", 60, 220, 22, LIGHTGRAY);
    EndDrawing();
}

void DrawGameOverScreen(bool vitoria) {
    BeginDrawing();
    ClearBackground(BLACK);
    if (vitoria) {
        DrawText("VOCÊ VENCEU!", GetScreenWidth() / 2 - MeasureText("VOCÊ VENCEU!", 40) / 2, GetScreenHeight() / 2 - 20, 40, GREEN);
        DrawText("Pressione ENTER para continuar", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para continuar", 20) / 2, GetScreenHeight() / 2 + 30, 20, WHITE);
    } else {
        DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 40) / 2, GetScreenHeight() / 2 - 20, 40, RED);
        DrawText("Pressione ENTER para reiniciar ou B para voltar", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para reiniciar ou B para voltar", 20) / 2, GetScreenHeight() / 2 + 30, 20, WHITE);
    }
    EndDrawing();
}