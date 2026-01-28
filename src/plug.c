#include "plug.h"

#define RAYMATH_IMPLEMENTATION

#include <math.h>
#include <raymath.h>
#include <raylib.h>
#include <stdlib.h>

#define MAP_WIDTH 100
#define MAP_HEIGHT 100

typedef struct State {
  // do not move width and height, as they are set by memset in main  :)
  // I just neither want it to be non-opaque struct nor create functions
  // to set values directly (may do that with xmacro)
  int width;
  int height;
  Color background;
  Mesh landscape;
  Material mat;
  Matrix matr;
  Camera camera;
} State;

size_t plug_state_size(void) {
  return sizeof(State);
}

void plug_init(void *state) { 
  State *s = (State*)state;

  s->camera = (Camera){ 0 };
  s->camera.position = (Vector3){ 50.0f, 50.0f, 50.0f };
  s->camera.target = (Vector3){ 0.0f, 10.0f, 0.0f };
  s->camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
  s->camera.fovy = 45.0f;
  s->camera.projection = CAMERA_PERSPECTIVE;
 
  s->background = BLACK;
  s->mat = LoadMaterialDefault();
  s->matr = MatrixIdentity();
  Image heightmap = GenImagePerlinNoise(MAP_WIDTH, MAP_HEIGHT, 50, 50, 1.f);
  s->landscape = GenMeshHeightmap(heightmap, (Vector3){100,20,100});
  UnloadImage(heightmap);
}

void plug_update(void *state) {
  State *s = (State*)state;
  UpdateCamera(&s->camera, CAMERA_ORBITAL); 
}

void plug_draw(void *state) {
  State *s = (State*)state;
  BeginDrawing();
  ClearBackground(s->background);

  BeginMode3D(s->camera);

  DrawMesh(s->landscape, s->mat, s->matr);
  DrawGrid(20, 10.0f);

  EndMode3D();


  EndDrawing();
}

void plug_deinit(void *state) { 
  State *s = (State*)state;

  UnloadMaterial(s->mat);
  UnloadMesh(s->landscape);
}
