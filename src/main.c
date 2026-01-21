#include <raylib.h>

// Plugin/Hot-reload
//
// 1. both host and the plugins must include same headers
//
// 2. host will load .so/.dll file into itself with dlopen or LoadLibrary
//
// 3. while program is running, filewatcher or a user input triggers rebuilding
//    process of shared libary (plug)
//
// 4. most important part is to preserve data between uses

int main(void) {
    InitWindow(900, 600, "Plugin");
    SetTargetFPS(60);

    while(!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
