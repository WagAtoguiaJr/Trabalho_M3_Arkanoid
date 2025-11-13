#include "raylib.h"

#ifndef OBJETOS_H
#define OBJETOS_H

struct Paddle
{
    Vector2 posicao;
    Vector2 tamanho;
    float velocidade;
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












#endif // OBJETOS_H