#ifndef OBJECTS_H
#define OBJECTS_H

#include "raylib.h"

struct Paddle
{
    Vector2 posicao;
    Vector2 tamanho;
    float velocidade;
};

struct Brick
{
    Vector2 posicao;
    Vector2 tamanho;
    bool ativo;
    int vidas;
    bool lifePowerUp;
    bool sizePowerUp;
    bool velocPowerUp;
};

struct Ball
{
    Vector2 posicao;
    float raio;
    Vector2 velocidade;
    bool ativo;
};

void InitPaddle(Paddle &paddle, const Rectangle &playfield, int vidas);
Brick** InitBricks(int linhas, int qtd, const Rectangle &playfield , Vector2 tamanho, int vidas);
void SetLifePowerUp(Brick** blocos, int linhas, int qtd, int powerUps);
void SetSizePowerUp(Brick** blocos, int linhas, int qtd, int powerUps);
void SetVelocPowerUp(Brick** blocos, int linhas, int qtd, int powerUps);
void InitBall(Ball &bola, Paddle &paddle, float raio);

#endif // OBJECTS_H