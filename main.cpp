#include "raylib.h"
#include <iostream>
#include <cmath>
#include <ctime>

using namespace std;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define SCREEN_WIDTH  800 
#define  SCREEN_HEIGHT 600

#define RADIUS 20.0f
#define SPEED 250.0f

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Movimentação Básica com Raylib");
    SetTargetFPS(60);
    
    Vector2 pos = {SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT - 30.0f};
    Vector2 size = {RADIUS * 10, 15.0f};
    int lastPos = RIGHT;

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();

        if(IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A))
        {
            pos.x -= SPEED * dt;
            lastPos = LEFT;
            
        }
        else if(IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D))
        {
            pos.x += SPEED * dt;
            lastPos = RIGHT;
            
        }
        
        BeginDrawing();
        ClearBackground(BLACK);

        //DrawCircleV(pos, RADIUS, RED);
        DrawRectangleV(pos, size, RED);

        DrawFPS(SCREEN_WIDTH - 90, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

