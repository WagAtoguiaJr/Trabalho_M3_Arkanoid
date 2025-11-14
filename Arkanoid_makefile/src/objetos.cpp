#include "objetos.h"
#include "raylib.h"
#include <cstdlib>



void InitPaddle(Paddle &paddle, int screenWidth, int screenHeight, Color cor, int vidas = 5)
{
    paddle.posicao = (Vector2) {(screenWidth / 2.0f), (screenHeight * 7.0f / 8.0f)};
    paddle.tamanho = (Vector2) {screenHeight/10.0f, 20.0f};
    paddle.vidas = vidas;
    paddle.cor = cor;
}
Bloco* InitBlocos(int linhas, int qtd, int screenWidth, int screenHeight, Vector2 tamanho, int vidas = 1)
{
    Bloco* blocos = new Bloco[linhas * qtd];
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < qtd; j++)
        {
            int index = i * qtd + j;
            blocos[index].posicao = { j * (tamanho.x + 5) + 35, i * (tamanho.y + 5) + 50 };
            blocos[index].tamanho = tamanho;
            if (i % 3 == 0)
                blocos[index].cor = RED;
            else if (i % 3 == 1)
                blocos[index].cor = GREEN;
            else
                blocos[index].cor = BLUE;
            blocos[index].ativo = true;
            blocos[index].vidas = vidas;
        }
    }
    
    return blocos;
}
void InitBola(Bola &bola, Paddle &paddle, float raio, Vector2 velocidade, Color cor)
{

    bola.posicao = (Vector2) {paddle.posicao.x + paddle.tamanho.x / 2.0f, paddle.posicao.y - raio - 1.0f};
    bola.raio = raio;
    bola.velocidade = velocidade;
    bola.cor = cor;
}