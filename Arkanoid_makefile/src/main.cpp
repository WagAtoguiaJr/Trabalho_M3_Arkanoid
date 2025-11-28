#include "raylib.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <cmath>
#include <ctime>
#include <string>
#include <algorithm>

#include "objects.h"
#include "audio.h"
#include "menu.h"
#include "ranking.h"
#include "timer.h"

using namespace std;

#define SCREEN_WIDTH  800 
#define SCREEN_HEIGHT 450

#define RAIO 7.0f
#define BLOCOS_COLUNAS 10
#define BLOCOS_LINHAS 3

static Rectangle playfield = { 60.0f, 80.0f, SCREEN_WIDTH - 120.0f, SCREEN_HEIGHT - 160.0f };

static GameState state = MENU;
Paddle paddle;
int vidasPaddle = 3;
int fases = 1;
Ball bola;
Brick** blocos = nullptr;
Vector2 tamanhoBloco;

static int blocosDestruidosFase = 0;
static int fasePoints = 0;
static int currentDifficultyIndex = 0;
static float difficultyMultiplier = 1.0f;
static int currentScore = 0;

float modVelocBola = 1.0f; 

static bool scoreSaved = false;
static string inputPlayerName = "";
static bool gameOver = false;
static bool pauseGame = false;
static bool vitoria = false;

void InitGame(bool resetTimer = true);
void UnloadGame();
void Scoring();
void CollisionBallPaddle();
void CollisionBallBrick();
void CollisionBallWalls();
void UpdateGame();
void DrawGame();
void PlayGame();

inline float ClampF(float v, float a, float b) { 
    return (v < a) ? a : (v > b) ? b : v; 
}

inline Rectangle PaddleRect(const Paddle &p) {
    return { p.posicao.x - p.tamanho.x / 2.0f, p.posicao.y - p.tamanho.y / 2.0f, p.tamanho.x, p.tamanho.y };
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoid");
    SetTargetFPS(60);

    InitAudioSystem();
    LoadAudioAssets();

    InitMenu();

    bool shouldStartGame = false;

    while (!WindowShouldClose())
    {
        switch (state) {
            case MENU:
                UpdateMenu(state, fases, shouldStartGame);
                DrawMenu();
                if (shouldStartGame) {
                    InitGame(true);
                    ResetTimer();
                    StartTimer();
                    shouldStartGame = false;
                }
                break;

            case PLAYING:
                UpdateAudio();
                PlayGame();
                if (gameOver) {
                    state = GAMEOVER;
                    currentScore = currentScore + fasePoints;
                }
                break;

            case CONTROLS:
                DrawControlsScreen();    
                if (IsKeyPressed(KEY_B)) state = MENU;
                
                break;
            
            case RANKING:
                DrawRankingScreen();
                if (IsKeyPressed(KEY_B)) state = MENU;
                break;

            case GAMEOVER:
            {
                bool allowSave = (!vitoria) || (vitoria && fases >= 3);
                DrawGameOverScreen(vitoria, allowSave, scoreSaved, currentScore, inputPlayerName);

                if (allowSave) {
                    if (!scoreSaved) {
                        int key = GetKeyPressed();
                        while (key > 0) {
                            int c = key;
                            if ((c >= 32) && (c <= 126) && inputPlayerName.size() < 20) {
                                inputPlayerName.push_back((char)c);
                            }
                            key = GetKeyPressed();
                        }
                        if (IsKeyPressed(KEY_BACKSPACE) && !inputPlayerName.empty()) {
                            inputPlayerName.pop_back();
                        }

                        if (IsKeyPressed(KEY_ENTER)) {
                            if (inputPlayerName.empty()) inputPlayerName = "PLAYER";

                            time_t t = time(nullptr);
                            struct tm *ptm = localtime(&t);
                            char dateBuf[16] = "0000-00-00";
                            if (ptm) {
                                strftime(dateBuf, sizeof(dateBuf), "%Y-%m-%d", ptm);
                            }

                            RankEntry entry;
                            entry.date = dateBuf;
                            entry.name = inputPlayerName;
                            entry.score = currentScore;
                            entry.difficulty = currentDifficultyIndex;
                            entry.timeSeconds = GetElapsedTime();

                            AppendRankingCSV("rank/ranking.csv", entry);
                            scoreSaved = true;
                        }

                        if (IsKeyPressed(KEY_B)) {

                            fases = 1; 
                            vidasPaddle = 3; 
                            currentScore = 0;

                            UnloadGame();
                            state = MENU;
                            scoreSaved = false;
                            inputPlayerName.clear();
                            currentScore = 0;
                            StopTimer();
                        }
                    } else {
                        if (IsKeyPressed(KEY_ENTER)) {
                             
                            fases = 1; 
                            vidasPaddle = 3; 
                            currentScore = 0;

                            UnloadGame();
                            ResetTimer();
                            InitGame(true);
                            StartTimer();
                            scoreSaved = false;
                            inputPlayerName.clear();
                            state = PLAYING;
                        }

                        if (IsKeyPressed(KEY_B)) {
                            
                            fases = 1; 
                            vidasPaddle = 3; 
                            currentScore = 0;
                            
                            UnloadGame();
                            state = MENU;
                            scoreSaved = false;
                            inputPlayerName.clear();
                            currentScore = 0;
                            StopTimer();
                        }
                    }
                } else {
                    if (IsKeyPressed(KEY_ENTER)) {
                        StopTimer();
                        fases++;
                        UnloadGame();
                        InitGame(false);
                        StartTimer();
                        state = PLAYING;
                    }
                    if (IsKeyPressed(KEY_B)) {
                        UnloadGame();
                        state = MENU;
                        fases = 1; 
                        vidasPaddle = 3; 
                        currentScore = 0;
                        StopTimer();
                    }
                }
            }
                break;

            default:
                state = MENU;
                break;
        }
    }

    UnloadGame();
    UnloadAudioSystem();
    CloseWindow();
    return 0;
}

void InitGame(bool resetTimer)
{
    if (resetTimer) {
        ResetTimer();
        StartTimer();
    }
    
    gameOver = false;
    vitoria = false;
    pauseGame = false;
    scoreSaved = false;
    inputPlayerName.clear();
    blocosDestruidosFase = 0;
    fasePoints = 0;

    if (blocos != nullptr) {
        UnloadGame();
    }

    int resistenciaBloco = 1;
    int qtdPowerUpsLife = 2;
    int qtdPowerUpsSize = 2;
    int qtdPowerUpsVeloc = 2;

    switch (fases)
    {
    case 1:
        resistenciaBloco = 1;
        qtdPowerUpsLife = 2;
        qtdPowerUpsSize = 2;
        qtdPowerUpsVeloc = 2;
        break;
    case 2:
        resistenciaBloco = 2;
        qtdPowerUpsLife = 1;
        qtdPowerUpsSize = 1;
        qtdPowerUpsVeloc = 2;
        break;
    case 3:
        resistenciaBloco = 3;
        qtdPowerUpsLife = 1;
        qtdPowerUpsSize = 1;
        qtdPowerUpsVeloc = 1;
        break;
    default:
        break;
    }

    const float marginX = 30.0f;
    const float marginYTop = 60.0f;
    const float marginYBottom = 40.0f;
    playfield.x = marginX;
    playfield.y = marginYTop;
    playfield.width = (float)GetScreenWidth() - 2.0f * marginX;
    playfield.height = (float)GetScreenHeight() - marginYTop - marginYBottom;

    tamanhoBloco = (Vector2){ playfield.width / (float) BLOCOS_COLUNAS, 40.0f };

    InitPaddle(paddle, playfield, vidasPaddle);

    blocos = InitBricks(BLOCOS_LINHAS, BLOCOS_COLUNAS, playfield, tamanhoBloco, resistenciaBloco);
    SetLifePowerUp(blocos, BLOCOS_LINHAS, BLOCOS_COLUNAS, qtdPowerUpsLife);
    SetSizePowerUp(blocos, BLOCOS_LINHAS, BLOCOS_COLUNAS, qtdPowerUpsSize);
    SetVelocPowerUp(blocos, BLOCOS_LINHAS, BLOCOS_COLUNAS, qtdPowerUpsVeloc);

    InitBall(bola, paddle, RAIO);
    currentDifficultyIndex = GetMenuDifficulty();
    switch (currentDifficultyIndex) {
        case 0: 
            modVelocBola = difficultyMultiplier = 1.0f; 
            break;
        case 1: 
            modVelocBola = 1.25f;
            difficultyMultiplier = 1.5f;
            break;
        case 2: 
            modVelocBola = 1.5f; 
            difficultyMultiplier = 2.0f;
            break;
        default: modVelocBola = difficultyMultiplier = 1.0f; break;
    }
}

void CollisionBallPaddle()
{
    if (!bola.ativo) return;

    Rectangle paddleRect = PaddleRect(paddle);
    Vector2 bolaCentro = {bola.posicao.x, bola.posicao.y};

    if (CheckCollisionCircleRec(bolaCentro, bola.raio, paddleRect))
    {
        bola.velocidade.y *= -1.0f;
        float distancia = bola.posicao.x - paddle.posicao.x;
        float porcentagem = distancia / (paddle.tamanho.x / 2);
        bola.velocidade.x = porcentagem * 5.0f;
        PlayPaddleCollisionSound();

        bola.posicao.y = paddle.posicao.y - paddle.tamanho.y/2.0f - bola.raio - 1.0f;
    }
}

void CollisionBallBrick()
{
    int linhas = BLOCOS_LINHAS;
    int colunas = BLOCOS_COLUNAS;

    if (!bola.ativo) return;

    bool colisaoTratada = false;

    for (int i = 0; i < linhas && !colisaoTratada; i++)
    {
        for (int j = 0; j < colunas && !colisaoTratada; j++)
        {
            Brick &b = blocos[i][j];

            if (!b.ativo) continue;

            float halfW = b.tamanho.x * 0.5f;
            float halfH = b.tamanho.y * 0.5f;

            float dx = bola.posicao.x - b.posicao.x;
            float dy = bola.posicao.y - b.posicao.y;

            float sobreposX = (halfW + bola.raio) - fabsf(dx);
            float sobreposY = (halfH + bola.raio) - fabsf(dy);
            if (sobreposX > 0.0f && sobreposY > 0.0f)
            {
                b.vidas--;
                PlayBlockCollisionSound();

                if (b.vidas <= 0)
                {
                    b.ativo = false;
                    PlayBlockDestroySound();

                    blocosDestruidosFase++;
                    Scoring();

                    if(b.lifePowerUp)
                    {
                        vidasPaddle++;
                        PlayPowerupSound();
                    }
                    if(b.sizePowerUp)
                    {
                        paddle.tamanho.x *= 1.2f;
                        float halfW = paddle.tamanho.x / 2.0f;
                        paddle.posicao.x = ClampF(paddle.posicao.x, playfield.x + halfW, playfield.x + playfield.width - halfW);
                        PlayPowerupSound();
                    }
                    if(b.velocPowerUp)
                    {
                        paddle.velocidade *= 1.2f;
                        PlayPowerupSound();
                    }
                }

                if (sobreposX < sobreposY)
                {
                     bola.velocidade.x *= -1.0f;
                     if (dx > 0.0f) bola.posicao.x += sobreposX;
                     else bola.posicao.x -= sobreposX;
                }
                else
                {
                    bola.velocidade.y *= -1.0f;
                    if (dy > 0.0f) bola.posicao.y += sobreposY;
                    else bola.posicao.y -= sobreposY;
                }

                colisaoTratada = true;
            }
         }
    }
}

void CollisionBallWalls()
{
    float left = playfield.x + bola.raio;
    float right = playfield.x + playfield.width - bola.raio;
    float top = playfield.y + bola.raio;
    float bottom = playfield.y + playfield.height - bola.raio;

    if (bola.posicao.x <= left || bola.posicao.x >= right)
    {
        bola.velocidade.x *= -1;
        PlayWallCollisionSound();
        bola.posicao.x = ClampF(bola.posicao.x, left, right);
    }
    if (bola.posicao.y <= top)
    {
        bola.velocidade.y *= -1;
        PlayWallCollisionSound();
        bola.posicao.y = top;
    }
    if (bola.posicao.y >= bottom)
    {
        vidasPaddle--;
        bola.ativo = false;
        PlayLifeDownSound();
        bola.posicao = (Vector2) {paddle.posicao.x, paddle.posicao.y - paddle.tamanho.y/2.0f - bola.raio - 1.0f};
        bola.velocidade = (Vector2) {0, 0};
        if (vidasPaddle <= 0)
        {
            gameOver = true;
            StopTimer();
        }
    }
}

void UnloadGame()
{
    if (blocos == nullptr) return;
    for (int i = 0; i < BLOCOS_LINHAS; i++)
    {
        delete[] blocos[i];
    }
    delete[] blocos;
    blocos = nullptr;
}

void Scoring()
{
    fasePoints = (int) roundf((blocosDestruidosFase * 10) * (float) fases * difficultyMultiplier);
}

void UpdateGame()
{
    if(!gameOver)
    {
        if (IsKeyPressed('P')) {
            pauseGame = !pauseGame;
            if (pauseGame) {
                StopTimer();
            } else {
                StartTimer();
            }
        }

        if (!pauseGame)
        {
            if(IsKeyDown(KEY_LEFT)) paddle.posicao.x -= paddle.velocidade;
            if(IsKeyDown(KEY_RIGHT)) paddle.posicao.x += paddle.velocidade;

            float halfW = paddle.tamanho.x / 2.0f;
            paddle.posicao.x = ClampF(paddle.posicao.x, playfield.x + halfW, playfield.x + playfield.width - halfW);

            if(!bola.ativo && IsKeyPressed(KEY_SPACE))
            {
                bola.ativo = true;
                bola.velocidade = (Vector2) {0.0f, -5.0f};
            }

            if(bola.ativo){
                bola.posicao.x += bola.velocidade.x * modVelocBola;
                bola.posicao.y += bola.velocidade.y * modVelocBola;
            } else {
                bola.posicao = (Vector2) {paddle.posicao.x, paddle.posicao.y - paddle.tamanho.y/2 - bola.raio - 1.0f};
            }

            CollisionBallPaddle();
            CollisionBallBrick();
            CollisionBallWalls();

            if (vidasPaddle <= 0)
            {
                gameOver = true;
                StopTimer();

            } else {
                bool todosDestruidos = true;
                for (int i = 0; i < BLOCOS_LINHAS; i++)
                {
                    for (int j = 0; j < BLOCOS_COLUNAS; j++)
                    {
                        if (blocos[i][j].ativo)
                        {
                            todosDestruidos = false;
                            break;
                        }
                    }
                    if (!todosDestruidos) break;
                }

                if (todosDestruidos)
                {
                    gameOver = true;
                    vitoria = true;
                    StopTimer();
                }
            }
        }
    }
}

void DrawGame()
{
    BeginDrawing();
    ClearBackground(BLACK);

    DrawRectangleRec(playfield, DARKGRAY);

    if(!gameOver)
    {
        int linhas = BLOCOS_LINHAS;
        int colunas = BLOCOS_COLUNAS;
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                Brick &b = blocos[i][j];
                if (b.ativo)
                {
                    Color c = (b.vidas == 3) ? RED : (b.vidas == 2 ? ORANGE : YELLOW);
                    DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, c);

                    if (b.lifePowerUp)
                    {
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, PINK);
                        DrawText("L", (int)(b.posicao.x - 5), (int)(b.posicao.y - 10), 20, BLACK);
                    }
                    else if (b.sizePowerUp)
                    {
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, PURPLE);
                        DrawText("T", (int)(b.posicao.x - 5), (int)(b.posicao.y - 10), 20, BLACK);
                    }
                    else if (b.velocPowerUp)
                    {
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, GREEN);
                        DrawText("V", (int)(b.posicao.x - 5), (int)(b.posicao.y - 10), 20, BLACK);
                    }

                    DrawRectangleLines((int)(b.posicao.x - b.tamanho.x / 2), (int)(b.posicao.y - b.tamanho.y / 2), (int)b.tamanho.x, (int)b.tamanho.y, BLACK);
                }
            }
        }

        DrawRectangleV((Vector2){paddle.posicao.x - paddle.tamanho.x / 2, paddle.posicao.y - paddle.tamanho.y / 2}, paddle.tamanho, WHITE);

        DrawCircleV(bola.posicao, bola.raio, WHITE);
        DrawText(("Vidas: " + to_string(vidasPaddle)).c_str(), 10, 10, 20, WHITE);
        DrawText(("Score: " + to_string(currentScore + fasePoints)).c_str(), GetScreenWidth() - 160, 10, 20, WHITE);

        {
            string timeStr = FormatTime(GetElapsedTime());
            int timeW = MeasureText(timeStr.c_str(), 20);
            DrawText(timeStr.c_str(), GetScreenWidth()/2 - timeW/2 - 40, 10, 20, LIGHTGRAY);
        }

        if (pauseGame)
        {
            DrawText("PAUSE", GetScreenWidth() / 2 - MeasureText("PAUSE", 40) / 2, GetScreenHeight() / 2 - 10, 40, BLUE);
        }
    }
    EndDrawing();
}

void PlayGame()
{
    UpdateGame();
    DrawGame();
}