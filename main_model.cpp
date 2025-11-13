#include "raylib.h"
#include "test.h"
#include <string>

int main()
{
    const int screenWidth = 720;
    const int screenHeight = 760;
    InitWindow(screenWidth, screenHeight, "template-project");
    
    Texture2D background = LoadTexture("../assets/background-green.png");
    Texture2D paddleSm = LoadTexture("../assets/paddle-sm.png");
    Texture2D paddleLg = LoadTexture("../assets/paddle-lg.png");
    Texture2D paddleMd = LoadTexture("../assets/paddle-md.png");
    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(BLACK);

        DrawTexture(background, 0, 0, WHITE);
        DrawTexture(paddleSm, (screenWidth-300)/2, 760 - 300, WHITE);
        DrawTexture(paddleMd, (screenWidth-200)/2, 760 - 200, WHITE);
        DrawTexture(paddleLg, (screenWidth-100)/2, 760 - 100, WHITE);
        DrawText("Exemplo de Sprite como Background", 20, 20, 20, YELLOW);

        EndDrawing();
    }

    UnloadTexture(background);
    CloseWindow();
    return 0;
}
