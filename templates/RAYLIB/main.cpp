#include <raylib.h>

int main() {
    InitWindow(800, 600, "raylib + CMake");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);
        DrawText("Hello raylib!", 300, 280, 20, BLACK);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}