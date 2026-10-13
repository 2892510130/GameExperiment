#include <iostream>
#include <raylib.h>

#include "utils/log.hpp"

using namespace Utils;

int main(int argc, char** argv) {
    const int screenWidth = 1440;
    const int screenHeight = 960;

    InitWindow(screenWidth, screenHeight, "Game Experiment");

    auto image = LoadImage("resource/texture/scarfy.png");
    auto texture = LoadTextureFromImage(image);
    const float srcW = static_cast<float>(image.width) / 6;
    const float srcH = static_cast<float>(image.height);
    Rectangle frame_rec = { 0.0f, 0.0f, srcW, srcH };
    Vector2 frame_pos = { 480.0f, 360.0f };
    UnloadImage(image);

    SetTargetFPS(60);

    const float moveSpeed = 300.0f, frame_speed = 8.0f;
    const float frameInterval = 1.0f / frame_speed;

    int moveDir = 1, texture_id = 0;
    float animTimer = 0.0f;

    while (!WindowShouldClose()) {
        int dirNow = 0;
        if (IsKeyDown(KEY_RIGHT)) dirNow += 1;
        if (IsKeyDown(KEY_LEFT))  dirNow -= 1;

        if (dirNow != 0) moveDir = dirNow;

        frame_pos.x += dirNow * moveSpeed * GetFrameTime();

        if (frame_pos.x < 0.0f)
            frame_pos.x = 0.0f;
        if (frame_pos.x > screenWidth - srcW)
            frame_pos.x = screenWidth - srcW;

        animTimer += GetFrameTime();
        if (animTimer >= frameInterval) {
            animTimer -= frameInterval;
            texture_id = (texture_id + 1) % 6;
            if (dirNow == 0) texture_id = 5;
        }

        float frameX = texture_id * srcW;
        if (moveDir >= 0) {
            frame_rec = { frameX, 0.0f, srcW, srcH };
        } else {
            frame_rec = { frameX, 0.0f, -srcW, srcH };
        }

        BeginDrawing();
            ClearBackground(WHITE);
            DrawTextureRec(texture, frame_rec, frame_pos, WHITE);
        EndDrawing();
    }

    UnloadTexture(texture);
    CloseWindow();
}