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
#define  SCREEN_HEIGHT 600

#define RADIUS 20.0f
#define SPEED 250.0f

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Arkanoid");
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

        DrawRectangleV(pos, size, YELLOW);

        DrawFPS(SCREEN_WIDTH - 90, 10);
        EndDrawing();
    }
    CloseWindow();
    return 0;
}

