#include "objetos.h"
#include "raylib.h"
#include <cstdlib>



void InicPaddle(Paddle &paddle, int screenWidth, int screenHeight, int vidas)
{
    paddle.posicao = (Vector2) {(screenWidth / 2.0f), (screenHeight * 7.0f / 8.0f)};
    paddle.tamanho = (Vector2) {screenWidth/10.0f, 20.0f};
    paddle.vidas = vidas;
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
        }
    }
    
    return blocos;
}
void InicBola(Bola &bola, Paddle &paddle, float raio)
{
    bola.posicao = (Vector2) {paddle.posicao.x + paddle.tamanho.x / 2.0f, paddle.posicao.y - raio - 1.0f};
    bola.raio = raio;
    bola.velocidade = (Vector2) {0, 0};
    bola.ativo = false;
}