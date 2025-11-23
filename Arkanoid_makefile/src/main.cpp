#include "raylib.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include "objetos.h"
#include "audio.h"
#include "menu.h"

using namespace std;

#define SCREEN_WIDTH  800 
#define SCREEN_HEIGHT 450

#define RAIO 7.0f
#define BLOCOS_COLUNAS 10
#define BLOCOS_LINHAS 3

// Estado do jogo
static GameState state = MENU;

// Jogo
Paddle paddle;
int vidasPaddle = 3;
int fases = 1;
Bola bola;
Bloco** blocos = nullptr;
Vector2 tamanhoBloco;

float modVelocBola = 1.25f; 

// Flags auxiliares
static bool gameOver = false;
static bool pauseGame = false;
static bool vitoria = false;

// Protótipos locais
void InicializarJogo();
void DescarregarJogo();
void ColisaoBolaPaddle();
void ColisaoBolaBlocos();
void ColisaoBolaParedes();
void AtualizarJogo();
void DesenharJogo();
void RenderizarJogo();

void InicializarJogo()
{
    // garante descarregar blocos anteriores se existirem
    if (blocos != nullptr) {
        DescarregarJogo();
    }

    int resistenciaBloco = 1;
    int qtdPowerUpsLife = 3;
    int qtdPowerUpsSize = 2;
    int qtdPowerUpsVeloc = 2;

    switch (fases)
    {
    case 1:
        resistenciaBloco = 1;
        qtdPowerUpsLife = 3;
        qtdPowerUpsSize = 2;
        qtdPowerUpsVeloc = 2;
        break;
    case 2:
        resistenciaBloco = 2;
        qtdPowerUpsLife = 2;
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

    tamanhoBloco = (Vector2) {GetScreenWidth() / BLOCOS_COLUNAS, 40};
    InicPaddle(paddle, GetScreenWidth(), GetScreenHeight(), vidasPaddle);
    blocos = InicBlocos(BLOCOS_LINHAS, BLOCOS_COLUNAS, GetScreenWidth(), GetScreenHeight(), tamanhoBloco, resistenciaBloco);
    SetLifePowerUp(blocos, BLOCOS_LINHAS, BLOCOS_COLUNAS, qtdPowerUpsLife);
    SetSizePowerUp(blocos, BLOCOS_LINHAS, BLOCOS_COLUNAS, qtdPowerUpsSize);
    SetVelocPowerUp(blocos, BLOCOS_LINHAS, BLOCOS_COLUNAS, qtdPowerUpsVeloc);
    InicBola(bola, paddle, RAIO);

    // aplica dificuldade selecionada no menu
    int diff = GetMenuDifficulty();
    switch (diff) {
        case 0: modVelocBola = 1.0f; break;
        case 1: modVelocBola = 1.25f; break;
        case 2: modVelocBola = 1.5f; break;
        default: modVelocBola = 1.0f; break;
    }

    gameOver = false;
    pauseGame = false;
    vitoria = false;
}

void ColisaoBolaPaddle()
{
    if (!bola.ativo) return;

    Rectangle paddleRect = {paddle.posicao.x - paddle.tamanho.x / 2, paddle.posicao.y - paddle.tamanho.y / 2, paddle.tamanho.x, paddle.tamanho.y};
    Vector2 bolaCentro = {bola.posicao.x, bola.posicao.y};

    if (CheckCollisionCircleRec(bolaCentro, bola.raio, paddleRect))
    {
        bola.velocidade.y *= -1.0f;
        float distancia = bola.posicao.x - paddle.posicao.x;
        float porcentagem = distancia / (paddle.tamanho.x / 2);
        bola.velocidade.x = porcentagem * 5.0f;
        PlayPaddleCollisionSound();
    }
}

void ColisaoBolaBlocos()
{
    int linhas = BLOCOS_LINHAS;
    int colunas = BLOCOS_COLUNAS;

    if (!bola.ativo) return;

    // evita múltiplas colisões por frame (comportamento clássico)
    bool colisaoTratada = false;

    for (int i = 0; i < linhas && !colisaoTratada; i++)
    {
        for (int j = 0; j < colunas && !colisaoTratada; j++)
        {
            Bloco &b = blocos[i][j];

            if (!b.ativo) continue;

            // Metade das dimensões do bloco
            float halfW = b.tamanho.x * 0.5f;
            float halfH = b.tamanho.y * 0.5f;

            // Distância entre centros
            float dx = bola.posicao.x - b.posicao.x;
            float dy = bola.posicao.y - b.posicao.y;

            // Penetração (quanto o círculo e o retângulo se sobrepõem)
            float sobreposX = (halfW + bola.raio) - fabsf(dx);
            float sobreposY = (halfH + bola.raio) - fabsf(dy);

            // Colisão detectada
            if (sobreposX > 0.0f && sobreposY > 0.0f)
            {
                // Destrói o bloco
                b.vidas--;
                PlayBlockCollisionSound();

                if (b.vidas <= 0)
                {
                    b.ativo = false;
                    PlayBlockDestroySound();

                    if(b.lifePowerUp)
                    {
                        vidasPaddle++;
                        PlayPowerupSound();
                    }
                    if(b.sizePowerUp)
                    {
                        paddle.tamanho.x *= 1.2f;
                        PlayPowerupSound();
                    }
                    if(b.velocPowerUp)
                    {
                        paddle.velocidade *= 1.2f;
                        PlayPowerupSound();
                    }
                }

                // Reflete a velocidade com base na menor penetração (eixo de colisão principal)
                if (sobreposX < sobreposY)
                {
                     // Colisão mais horizontal -> inverter velocidade X
                     bola.velocidade.x *= -1.0f;
                    // Ajuste de posição para evitar ficar "dentro" do bloco
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

void ColisaoBolaParedes()
{
    if (bola.posicao.x - bola.raio <= 0 || bola.posicao.x + bola.raio >= GetScreenWidth())
    {
        bola.velocidade.x *= -1;
        PlayWallCollisionSound();
    }
    if (bola.posicao.y - bola.raio <= 0)
    {
        bola.velocidade.y *= -1;
        PlayWallCollisionSound();
    }
    if (bola.posicao.y + bola.raio >= GetScreenHeight())
    {
        vidasPaddle--;
        bola.ativo = false;
        PlayLifeDownSound();
        bola.posicao = (Vector2) {paddle.posicao.x + paddle.tamanho.x / 2.0f, paddle.posicao.y - bola.raio - 1.0f};
        bola.velocidade = (Vector2) {0, 0};
        if (vidasPaddle <= 0)
        {
            gameOver = true;
        }
    }
}

void DescarregarJogo()
{
    if (blocos == nullptr) return;
    for (int i = 0; i < BLOCOS_LINHAS; i++)
    {
        free(blocos[i]);
    }
    free(blocos);
    blocos = nullptr;
}

void AtualizarJogo()
{
    if(!gameOver)
    {
        if (IsKeyPressed('P')) pauseGame = !pauseGame;

        if (!pauseGame)
        {
            if(IsKeyDown(KEY_LEFT)) paddle.posicao.x -= paddle.velocidade;
            if((paddle.posicao.x - paddle.tamanho.x/2) <= 0) paddle.posicao.x = paddle.tamanho.x/2;
            if(IsKeyDown(KEY_RIGHT)) paddle.posicao.x += paddle.velocidade;
            if((paddle.posicao.x + paddle.tamanho.x/2) >= GetScreenWidth()) paddle.posicao.x = GetScreenWidth() - paddle.tamanho.x/2;

            if(!bola.ativo && IsKeyPressed(KEY_SPACE))
            {
                bola.ativo = true;
                bola.velocidade = (Vector2) {0.0, -5.0f};
            }

            if(bola.ativo){
                bola.posicao.x += bola.velocidade.x * modVelocBola;
                bola.posicao.y += bola.velocidade.y * modVelocBola;
            } else {
                bola.posicao = (Vector2) {paddle.posicao.x, paddle.posicao.y - paddle.tamanho.y/2 - bola.raio - 1.0f};
            }

            ColisaoBolaPaddle();
            ColisaoBolaBlocos();
            ColisaoBolaParedes();

            if (vidasPaddle <= 0)
            {
                gameOver = true;
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
                }
            }
        }
    }
}

void DesenharJogo()
{
    BeginDrawing();
    ClearBackground(BLACK);

    if(!gameOver)
    {
        DrawRectangleV((Vector2){paddle.posicao.x - paddle.tamanho.x / 2, paddle.posicao.y - paddle.tamanho.y / 2}, paddle.tamanho, WHITE);
        DrawCircleV(bola.posicao, bola.raio, WHITE);

        int linhas = BLOCOS_LINHAS;
        int colunas = BLOCOS_COLUNAS;
        for (int i = 0; i < linhas; i++)
        {
            for (int j = 0; j < colunas; j++)
            {
                Bloco &b = blocos[i][j];
                if (b.ativo)
                {
                    if (b.vidas == 3)
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, RED);
                    else if (b.vidas == 2)
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, ORANGE);
                    else
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, YELLOW);

                    if (b.lifePowerUp)
                    {
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, PINK);
                        DrawText("L", b.posicao.x - 5, b.posicao.y - 10, 20, BLACK);
                    }
                    else if (b.sizePowerUp)
                    {
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, PURPLE);
                        DrawText("S", b.posicao.x - 5, b.posicao.y - 10, 20, BLACK);
                    }
                    else if (b.velocPowerUp)
                    {
                        DrawRectangleV((Vector2){b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2}, b.tamanho, GREEN);
                        DrawText("V", b.posicao.x - 5, b.posicao.y - 10, 20, BLACK);
                    }

                    DrawRectangleLines(b.posicao.x - b.tamanho.x / 2, b.posicao.y - b.tamanho.y / 2, b.tamanho.x, b.tamanho.y, BLACK);
                }
            }
        }

        DrawText(("Vidas: " + to_string(vidasPaddle)).c_str(), 10, 10, 20, WHITE);

        if (pauseGame)
        {
            DrawText("PAUSE", GetScreenWidth() / 2 - MeasureText("PAUSE", 40) / 2, GetScreenHeight() / 2 - 20, 40, YELLOW);
        }
    }
    else
    {
        // tela de gameover é desenhada pelo menu (DrawGameOverScreen)
    }

    EndDrawing();
}

void RenderizarJogo()
{
    AtualizarJogo();
    DesenharJogo();
}

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoid");
    SetTargetFPS(60);

    // Inicializa áudio e assets
    InitAudioSystem();
    LoadAudioAssets();

    // Inicializa menu
    InitMenu();

    bool shouldStartGame = false;

    while (!WindowShouldClose())
    {
        switch (state) {
            case MENU:
                UpdateMenu(state, fases, shouldStartGame);
                DrawMenu();
                if (shouldStartGame) {
                    // inicia jogo com a dificuldade selecionada
                    InicializarJogo();
                    shouldStartGame = false;
                }
                break;

            case PLAYING:
                UpdateAudio(); // atualiza stream de música
                RenderizarJogo();
                if (gameOver) {
                    state = GAMEOVER;
                }
                break;

            case CONTROLS:
                if (IsKeyPressed(KEY_B)) state = MENU;
                DrawControlsScreen();
                break;

            case GAMEOVER:
                DrawGameOverScreen(vitoria);
                if (IsKeyPressed(KEY_ENTER)) {
                    if (vitoria) {
                        fases++;
                    } else {
                        fases = 1;
                        vidasPaddle = 3;
                    }
                    DescarregarJogo();
                    InicializarJogo();
                    gameOver = false;
                    vitoria = false;
                    state = PLAYING;
                }
                if (IsKeyPressed(KEY_B) && !vitoria) {
                    // volta ao menu sem reiniciar
                    DescarregarJogo();
                    InicializarJogo();
                    fases = 1;
                    vidasPaddle = 3;
                    gameOver = false;
                    vitoria = false;
                    state = MENU;
                }
                break;

            default:
                state = MENU;
                break;
        }
    }

    // limpeza
    DescarregarJogo();
    UnloadAudioSystem();
    CloseWindow();
    return 0;
}