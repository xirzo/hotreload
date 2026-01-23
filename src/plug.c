#include "plug.h"

#include <raylib.h>
#include <stdlib.h>

typedef struct State {
    Color background;
} State;

size_t plug_state_size(void) {
    return sizeof(State);
}

void plug_init(void *state) { 
    ((State*)state)->background = RED;
}

void plug_update(void *state) { }

void plug_draw(void *state) {
    State *s = (State*)state;
    BeginDrawing();
    ClearBackground(s->background);

    DrawText("Hello, SHASA!", 0, 0, 20, BLACK);

    EndDrawing();
}

void plug_deinit(void *state) { 
    (void)state;
}
