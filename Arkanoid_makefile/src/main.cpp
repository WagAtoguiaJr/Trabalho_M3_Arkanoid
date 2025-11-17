#include "raylib.h"
#include <iostream>
#include <cmath>
#include <ctime>
#include <string>
#include "objetos.h"

using namespace std;

#define LEFT 2
#define RIGHT 3

#define SCREEN_WIDTH  800 
#define  SCREEN_HEIGHT 450

#define RAIO 7.0f
#define BLOCOS_COLUNAS 5
#define BLOCOS_LINHAS 1

static bool gameOver = false;
static bool pause = false;
static bool vitoria = false;

Paddle paddle;
Bola bola;
Bloco** blocos;

Vector2 tamanhoBloco;

void InicializarJogo()
{
    tamanhoBloco = (Vector2) {GetScreenWidth() / BLOCOS_COLUNAS, 40};
    InicPaddle(paddle, GetScreenWidth(), GetScreenHeight());
    blocos = InicBlocos(BLOCOS_LINHAS, BLOCOS_COLUNAS, GetScreenWidth(), GetScreenHeight(), tamanhoBloco);
    InicBola(bola, paddle, RAIO);
}

void ColisaoBolaPaddle()
{
    if (!bola.ativo) return;

    Rectangle paddleRect = {paddle.posicao.x - paddle.tamanho.x / 2, paddle.posicao.y - paddle.tamanho.y / 2, paddle.tamanho.x, paddle.tamanho.y};
    Vector2 bolaCentro = {bola.posicao.x, bola.posicao.y};

    if (CheckCollisionCircleRec(bolaCentro, bola.raio, paddleRect))
    {
        bola.velocidade.y *= -1;
        float distancia = bola.posicao.x - paddle.posicao.x;
        float porcentagem = distancia / (paddle.tamanho.x / 2);
        bola.velocidade.x = porcentagem * 5.0f;
    }
}

void ColisaoBolaBlocos()
{
    int linhas = BLOCOS_LINHAS;
    int colunas = BLOCOS_COLUNAS;

    if (!bola.ativo) return;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
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
                b.ativo = false;

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
             }
         }
    }
}

void ColisaoBolaParedes()
{
    if (bola.posicao.x - bola.raio <= 0 || bola.posicao.x + bola.raio >= GetScreenWidth())
    {
        bola.velocidade.x *= -1;
    }
    if (bola.posicao.y - bola.raio <= 0)
    {
        bola.velocidade.y *= -1;
    }
    if (bola.posicao.y + bola.raio >= GetScreenHeight())
    {
        paddle.vidas--;
        bola.ativo = false;
        bola.posicao = (Vector2) {paddle.posicao.x + paddle.tamanho.x / 2.0f, paddle.posicao.y - bola.raio - 1.0f};
        bola.velocidade = (Vector2) {0, 0};
        if (paddle.vidas <= 0)
        {
            gameOver = true;
        }
    }
}

void AtualizarJogo()
{
    if(!gameOver)
    {
        if (IsKeyPressed('P')) pause = !pause;

        if (!pause)
        {
            if(IsKeyDown(KEY_LEFT)) paddle.posicao.x -= 5.0f;
            if((paddle.posicao.x - paddle.tamanho.x/2) <= 0) paddle.posicao.x = paddle.tamanho.x/2;
            if(IsKeyDown(KEY_RIGHT)) paddle.posicao.x += 5.0f;
            if((paddle.posicao.x + paddle.tamanho.x/2) >= GetScreenWidth()) paddle.posicao.x = GetScreenWidth() - paddle.tamanho.x/2;

            if(!bola.ativo && IsKeyPressed(KEY_SPACE))
            {
                bola.ativo = true;
                bola.velocidade = (Vector2) {0.0, -5.0f};
            }

            if(bola.ativo){
                    bola.posicao.x += bola.velocidade.x;
                    bola.posicao.y += bola.velocidade.y;
                
            }else{
                    bola.posicao = (Vector2) {paddle.posicao.x, paddle.posicao.y - paddle.tamanho.y/2 - bola.raio - 1.0f};
            }
            
            ColisaoBolaPaddle();
            ColisaoBolaBlocos();
            ColisaoBolaParedes();
           
            if (paddle.vidas <= 0)
            {
                gameOver = true;
            }else{
                
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
    } else {
        if (IsKeyPressed(KEY_ENTER))
        {
            InicializarJogo();
            gameOver = false;
            vitoria = false;
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
                    if(j % 2 == 0)
                        DrawRectangle(b.posicao.x - b.tamanho.x/2, b.posicao.y - b.tamanho.y/2, b.tamanho.x, b.tamanho.y, BLUE);
                    else
                        DrawRectangle(b.posicao.x - b.tamanho.x/2, b.posicao.y - b.tamanho.y/2, b.tamanho.x, b.tamanho.y, RED);
                }
            }
        }

        DrawText(("Vidas: " + to_string(paddle.vidas)).c_str(), 10, 10, 20, WHITE);

        if (pause)
        {
            DrawText("PAUSE", GetScreenWidth() / 2 - MeasureText("PAUSE", 40) / 2, GetScreenHeight() / 2 - 20, 40, YELLOW);
        }
    }
    else
    {
        if (vitoria)
        {
            DrawText("VOCÊ VENCEU!", GetScreenWidth() / 2 - MeasureText("VOCÊ VENCEU!", 40) / 2, GetScreenHeight() / 2 - 20, 40, GREEN);
            DrawText("Pressione ENTER para reiniciar", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para reiniciar", 20) / 2, GetScreenHeight() / 2 + 30, 20, WHITE);
        }
        else
        {
            DrawText("GAME OVER", GetScreenWidth() / 2 - MeasureText("GAME OVER", 40) / 2, GetScreenHeight() / 2 - 20, 40, RED);
            DrawText("Pressione ENTER para reiniciar", GetScreenWidth() / 2 - MeasureText("Pressione ENTER para reiniciar", 20) / 2, GetScreenHeight() / 2 + 30, 20, WHITE);
        }
    }
    
    EndDrawing();
}

void RenderizarJogo()
{
    AtualizarJogo();
    DesenharJogo();
}

void DescarregarJogo()
{
    int linhas = (int)BLOCOS_LINHAS;
    for (int i = 0; i < linhas; i++)
    {
        delete[] blocos[i];
    }
    delete[] blocos;
}


int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoid");
    SetTargetFPS(60);
    InicializarJogo();

    while (!WindowShouldClose())
    {
        RenderizarJogo();
    }

    DescarregarJogo();

    CloseWindow();
    return 0;
}

