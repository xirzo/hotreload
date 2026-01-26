#include "plug.h"

#define RAYMATH_IMPLEMENTATION
#include <raymath.h>
#include <raylib.h>
#include <stdlib.h>

typedef struct State {
    Color background;
} State;

size_t plug_state_size(void) {
    return sizeof(State);
}

void plug_init(void *state) { 
    State *s = (State*)state;
    s->background = WHITE;
}

void plug_update(void *state) { 
    State *s = (State*)state;
}

void plug_draw(void *state) {
    State *s = (State*)state;
    BeginDrawing();
    ClearBackground(s->background);

    EndDrawing();
}

void plug_deinit(void *state) { 
    (void)state;
}
