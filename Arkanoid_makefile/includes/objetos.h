#include "raylib.h"

#ifndef OBJETOS_H
#define OBJETOS_H

struct Paddle
{
    Vector2 posicao;
    Vector2 tamanho;
    float velocidade;
};

struct Bloco
{
    Vector2 posicao;
    Vector2 tamanho;
    bool ativo;
    int vidas;
    bool lifePowerUp;
    bool sizePowerUp;
    bool velocPowerUp;
};

struct Bola
{
    Vector2 posicao;
    float raio;
    Vector2 velocidade;
    bool ativo;
};

void InicPaddle(Paddle &paddle, int screenWidth, int screenHeight, int vidas = 3);
Bloco** InicBlocos(int linhas, int qtd, int screenWidth, int screenHeight, Vector2 tamanho, int vidas = 1);
void SetLifePowerUp(Bloco** blocos, int linhas, int qtd, int powerUps);
void SetSizePowerUp(Bloco** blocos, int linhas, int qtd, int powerUps);
void SetVelocPowerUp(Bloco** blocos, int linhas, int qtd, int powerUps);
void InicBola(Bola &bola, Paddle &paddle, float raio);












#endif // OBJETOS_H