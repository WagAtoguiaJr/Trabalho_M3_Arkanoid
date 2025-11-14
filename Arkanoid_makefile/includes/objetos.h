#include "raylib.h"

#ifndef OBJETOS_H
#define OBJETOS_H

struct Paddle
{
    Vector2 posicao;
    Vector2 tamanho;
    int vidas;
    Color cor;

};

struct Bloco
{
    Vector2 posicao;
    Vector2 tamanho;
    Color cor;
    bool ativo;
    int vidas;
};

struct Bola
{
    Vector2 posicao;
    float raio;
    Vector2 velocidade;
    Color cor;
};

void InitPaddle(Paddle &paddle, int screenWidth, int screenHeight, Color cor, int vidas = 5);
Bloco* InitBlocos(int linhas, int qtd, int screenWidth, int screenHeight, Vector2 tamanho, int vidas = 1);
void InitBola(Bola &bola, Paddle &paddle, float raio, Vector2 velocidade, Color cor);












#endif // OBJETOS_H