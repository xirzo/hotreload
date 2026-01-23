#ifndef PLUG_H
#define PLUG_H

#include <stddef.h>

typedef struct {
    void (*plug_init)(void *state);
    void (*plug_update)(void *state);
    void (*plug_draw)(void *state);
    void (*plug_deinit)(void *state);
    size_t (*plug_state_size)(void);
} Plug;

#endif // PLUG_H
