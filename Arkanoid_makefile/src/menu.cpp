#include "menu.h"
#include "audio.h" // chama PlayMenuNavigateSound / PlayMenuSelectSound
#include <iostream>
#include <vector>
#include <ctime>
#include <string>

using namespace std;

static int menuIndex = 0;
static const char* menuItems[] = { "Iniciar Jogo", "Dificuldade", "Controles", "Ranking", "Sair" };
static const int MENU_COUNT = 5;
static int difficulty = 0; // 0=Fácil,1=Médio,2=Difícil

void InitMenu() {
    menuIndex = 0;
    difficulty = 0;
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
            case 3: // Pontuação / Ranking
                state = RANKING;
                break;
            case 4: // Sair
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

void DrawGameOverScreen(const bool &vitoria, const bool &allowSave, const bool &scoreSaved, const std::string &inputPlayerName)
{
    const int screenW = GetScreenWidth();
    const int screenH = GetScreenHeight();

    // Margens e espaçamentos
    const int marginX = 60;
    const int bottomMargin = 60;
    const int titleSize = 40;
    const int subtitleSize = 30;
    const int textSize = 20;
    const int inputSize = 30;
    const int lineSpacing = 8; // espaço extra entre linhas

    BeginDrawing();
    ClearBackground(BLACK);

    // --- Título (centralizado)
    const char *titleText = vitoria ? "VOCÊ VENCEU!" : "GAME OVER";
    Color titleColor = vitoria ? GREEN : RED;
    int titleW = MeasureText(titleText, titleSize);
    int titleX = screenW / 2 - titleW / 2;
    int titleY = screenH / 6; // coloca o título no terço superior
    DrawText(titleText, titleX, titleY, titleSize, titleColor);

    // calcula posição inicial para o bloco de conteúdo abaixo do título
    int y = titleY + titleSize + 24; // 24 px de folga após o título

    // --- Se pode salvar (derrota ou fim do jogo)
    if (allowSave)
    {
        if (!scoreSaved)
        {
            // Linha de instrução
            DrawText("Digite seu nome e pressione ENTER para salvar:", marginX, y, textSize, WHITE);
            int instrH = textSize;
            y += instrH + lineSpacing;

            // Campo de input (nome do jogador)
            DrawText(inputPlayerName.c_str(), marginX, y, inputSize, YELLOW);
            int inputH = inputSize;
            y += inputH + (lineSpacing * 2);

            // Instrução de voltar (na parte inferior, alinhada à margem)
            DrawText("Pressione B para voltar ao menu sem salvar", marginX, screenH - bottomMargin, 18, LIGHTGRAY);
        }
        else
        {
            // Mensagem centralizada de "Score salvo!"
            const char *savedText = "Score salvo!";
            int savedW = MeasureText(savedText, subtitleSize);
            DrawText(savedText, screenW / 2 - savedW / 2, y, subtitleSize, GREEN);
            y += subtitleSize + 18;

            // Ações disponíveis (centralizadas)
            const char *contText = "Pressione ENTER para reiniciar / continuar";
            int contW = MeasureText(contText, textSize);
            DrawText(contText, screenW / 2 - contW / 2, y, textSize, WHITE);
            y += textSize + lineSpacing;

            const char *backText = "Pressione B para voltar ao menu";
            int backW = MeasureText(backText, textSize);
            DrawText(backText, screenW / 2 - backW / 2, y, textSize, LIGHTGRAY);
        }
    }
    else
    {
        // Fase concluída (mensagens centralizadas)
        const char *doneText = "Fase concluida!";
        int doneW = MeasureText(doneText, subtitleSize);
        DrawText(doneText, screenW / 2 - doneW / 2, y, subtitleSize, GREEN);
        y += subtitleSize + 18;

        const char *nextText = "Pressione ENTER para ir para a proxima fase";
        int nextW = MeasureText(nextText, textSize);
        DrawText(nextText, screenW / 2 - nextW / 2, y, textSize, WHITE);
        y += textSize + lineSpacing;

        const char *backText = "Pressione B para voltar ao menu";
        int backW = MeasureText(backText, textSize);
        DrawText(backText, screenW / 2 - backW / 2, y, textSize, LIGHTGRAY);
    }

    EndDrawing();
}