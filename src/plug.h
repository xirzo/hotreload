#ifndef PLUG_H
#define PLUG_H

typedef struct {
    void (*plug_update)(void);
    void (*plug_draw)(void);
} Plug;

#endif // PLUG_H
