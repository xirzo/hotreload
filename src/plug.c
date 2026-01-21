#include "plug.h"

#include <raylib.h>

void plug_update(void) {
}

void plug_draw(void) {
    BeginDrawing();
    ClearBackground(WHITE);

    DrawText("Hello, SHASA!", 0, 0, 20, BLACK);

    EndDrawing();
}
