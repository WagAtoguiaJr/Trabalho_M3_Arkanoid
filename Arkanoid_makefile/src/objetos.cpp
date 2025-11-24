#include "objetos.h"
#include "raylib.h"
#include <cstdlib>

void InicPaddle(Paddle &paddle, const Rectangle &playfield, int vidas)
{
    paddle.posicao = (Vector2) {(playfield.x + playfield.width/ 2), (playfield.y + playfield.height - 24.0f)};
    paddle.tamanho = (Vector2) {playfield.width / 8.0f, 20.0f};
    paddle.velocidade = 6.0f;
}

Bloco** InicBlocos(int linhas, int qtd, const Rectangle &playfield , Vector2 tamanho, int vidas)
{
    Bloco** blocos = new Bloco*[linhas];
    // calcula margem superior para os blocos (por exemplo 8% do playfield height)
    float topMargin = playfield.y + 50.0f; // ou playfield.y + playfield.height * 0.05f;
    for (int i = 0; i < linhas; i++)
    {
        blocos[i] = new Bloco[qtd];
        for (int j = 0; j < qtd; j++)
        {
            // posição relativa dentro do playfield
            float x = playfield.x + j * tamanho.x + tamanho.x / 2.0f;
            float y = topMargin + i * tamanho.y + tamanho.y / 2.0f;
            blocos[i][j].posicao = (Vector2){ x, y };
            blocos[i][j].tamanho = tamanho;
            blocos[i][j].ativo = true;
            blocos[i][j].vidas = vidas;
            blocos[i][j].lifePowerUp = false;
            blocos[i][j].sizePowerUp = false;
            blocos[i][j].velocPowerUp = false;
        }
    }
    return blocos;
}

void SetLifePowerUp(Bloco** blocos, int linhas, int qtd, int powerUps)
{
    if (powerUps == 0) return;

    int randomLinha = rand() % linhas;
    int randomColuna = rand() % qtd;
    Bloco &b = blocos[randomLinha][randomColuna];
    
    if (b.ativo && !b.lifePowerUp && !b.sizePowerUp && !b.velocPowerUp)
    {
        b.lifePowerUp = true;
        b.vidas = 1;
    }
    else
    {
        SetLifePowerUp(blocos, linhas, qtd, powerUps);
    }

    SetLifePowerUp(blocos, linhas, qtd, powerUps - 1);
}

void SetSizePowerUp(Bloco** blocos, int linhas, int qtd, int powerUps)
{
    if (powerUps == 0) return;
    
    int randomLinha = rand() % linhas;
    int randomColuna = rand() % qtd;
    Bloco &b = blocos[randomLinha][randomColuna];
    
    if (b.ativo && !b.sizePowerUp && !b.lifePowerUp && !b.velocPowerUp)
    {
        b.sizePowerUp = true;
        b.vidas = 1;
    }
    else
    {
        SetSizePowerUp(blocos, linhas, qtd, powerUps);
    }

   SetSizePowerUp(blocos, linhas, qtd, powerUps - 1);
}

void SetVelocPowerUp(Bloco** blocos, int linhas, int qtd, int powerUps)
{
    if (powerUps == 0) return;
    
    int randomLinha = rand() % linhas;
    int randomColuna = rand() % qtd;
    Bloco &b = blocos[randomLinha][randomColuna];
    
    if (b.ativo && !b.velocPowerUp && !b.sizePowerUp && !b.lifePowerUp)
    {
        b.velocPowerUp = true;
        b.vidas = 1;
    }
    else
    {
        SetVelocPowerUp(blocos, linhas, qtd, powerUps);
    }

   SetVelocPowerUp(blocos, linhas, qtd, powerUps - 1);
}

void InicBola(Bola &bola, Paddle &paddle, float raio)
{
    bola.posicao = (Vector2) {paddle.posicao.x, paddle.posicao.y - raio - 1.0f};
    bola.raio = raio;
    bola.velocidade = (Vector2) {0, 0};
    bola.ativo = false;
}