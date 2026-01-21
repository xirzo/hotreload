#include <stdio.h>
#include <raylib.h>

#include "plug.h"
#include "hotreload.h"

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
    SetTraceLogLevel(LOG_ERROR);
    InitWindow(900, 600, "Plugin");
    SetTargetFPS(60);

    Plug plug = {0};

    if (plug_load(&plug) != 0) {
        fprintf(stderr, "ERROR: Failed to initially load the plug\n");
        CloseWindow();
        return 1;
    }

    while(!WindowShouldClose()) {
        if (IsKeyPressed(KEY_ESCAPE)) {
            break;
        }

        if (IsKeyPressed(KEY_SPACE) && plug_reload(&plug) != 0) {
            fprintf(stderr, "ERROR: Failed to reload the plug\n");
            CloseWindow();
            return 1;
        }

        plug.plug_draw();
    }

    if (plug_unload(&plug) != 0) {
        fprintf(stderr, "ERROR: Failed to unload the plug\n");
        return 1;
    }

    CloseWindow();
    return 0;
}
