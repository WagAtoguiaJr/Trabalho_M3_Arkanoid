#include "objetos.h"
#include "raylib.h"
#include <cstdlib>

void InicPaddle(Paddle &paddle, int screenWidth, int screenHeight, int vidas)
{
    paddle.posicao = (Vector2) {(screenWidth / 2.0f), (screenHeight * 7.0f / 8.0f)};
    paddle.tamanho = (Vector2) {screenWidth/10.0f, 20.0f};
    paddle.velocidade = 5.0f;
}

Bloco** InicBlocos(int linhas, int qtd, int screenWidth, int screenHeight, Vector2 tamanho, int vidas)
{
    Bloco** blocos = new Bloco*[linhas];
    for (int i = 0; i < linhas; i++)
    {
        blocos[i] = new Bloco[qtd];
        for (int j = 0; j < qtd; j++)
        {
            blocos[i][j].posicao = (Vector2){j*tamanho.x + tamanho.x/2, i*tamanho.y + 50 + tamanho.y/2};
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
    }
    else
    {
        SetVelocPowerUp(blocos, linhas, qtd, powerUps);
    }

   SetVelocPowerUp(blocos, linhas, qtd, powerUps - 1);
}

void InicBola(Bola &bola, Paddle &paddle, float raio)
{
    bola.posicao = (Vector2) {paddle.posicao.x + paddle.tamanho.x / 2.0f, paddle.posicao.y - raio - 1.0f};
    bola.raio = raio;
    bola.velocidade = (Vector2) {0, 0};
    bola.ativo = false;
}