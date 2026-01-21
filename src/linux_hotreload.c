#include "hotreload.h"

#include <assert.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <stdio.h>
#include <raylib.h>

static const char *rebuild_script_name = "./rebuild_plug.sh";
static const char *lib_name = "./plug.so";

static void* lib = NULL;

int plug_rebuild() {
    int code = system(rebuild_script_name);

    if (code != 0) {
        fprintf(stderr, "ERROR: Failed to rebuild linux plug\n");
        return code;
    }

    return 0;
}

int plug_load(Plug *plug) {
    printf("Linux plug reloading...\n");

    int rebuild_code;
    if ((rebuild_code = plug_rebuild()) != 0) {
        return rebuild_code;
    }

    lib = dlopen(lib_name, RTLD_NOW);

    if (lib == NULL) {
        fprintf(stderr, "ERROR: Failed to load library after rebuild: %s\n", dlerror());
        return 1;
    }

    plug->plug_draw = dlsym(lib, "plug_draw");

    char *error;
    if ((error = dlerror()) != NULL)  {
        fprintf (stderr, "ERROR: Failed to load %s function: %s\n", "plug_draw", error);
        return 1;
    }

    printf("Successfully loaded the plug\n");
    return 0;
}

int plug_reload(Plug *plug) {
    if (lib == NULL) {
        fprintf(stderr, "ERROR: lib is not loaded yet\n");
        return 1;
    }

    assert(plug_unload(plug) == 0);
    assert(plug_load(plug) == 0);

    printf("Successfully reloaded the plug\n");
    return 0;
}

int plug_unload(Plug *plug) {
    if (lib == NULL) {
        fprintf(stderr, "ERROR: lib is not loaded yet\n");
        return 1;
    }

    int dlclose_result = dlclose(lib);
    if (dlclose_result != 0) {
        fprintf(stderr, "ERROR: Failed to close plug: %s\n", dlerror());
        return dlclose_result;
    }
    
    printf("Successfully unloaded the plug\n");
    return 0;
}
