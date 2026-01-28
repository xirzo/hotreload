#include "plug.h"

#define RAYMATH_IMPLEMENTATION

#include <rlgl.h>
#include <stdio.h>
#include <math.h>
#include <raymath.h>
#include <raylib.h>
#include <stdlib.h>

#define LTOP (Color){87, 73, 100, 255}
#define LMID (Color){159, 131, 131, 255}
#define LBOTMID (Color){200, 170, 170, 255}
#define LBOT (Color){255, 218, 179, 255}

#define MAP_WIDTH 170
#define MAP_HEIGHT 170

#define MAP_MESH_WIDTH 100
#define MAP_MESH_HEIGHT 100

#define MAP_FIRST_PERLIN_SCALE 1.0f
#define MAP_SECOND_PERLIN_SCALE 20.0f

// TODO: add noise for randomization
// TODO: add color-height gradient

typedef struct State {
  // do not move width, height, camera, as they are set by memset in main  :)
  // I just neither want it to be non-opaque struct nor create functionа
  // to set values directly (may do that with xmacro)
  int width;
  int height;
  Camera camera;
  Color background;
  Model landscape;
} State;

size_t plug_state_size(void) {
  return sizeof(State);
}

Color calculate_color_by_height(float height) {
  if (height < 150) {
    return LBOT;
  }

  if (height >= 150 && height < 187) {
    return LMID;
  }

  if (height >= 187) {
    return LTOP;
  }

  return BLACK;
}

void plug_init(void *state) { 
  State *s = (State*)state;
 
  s->background = SKYBLUE;

  Image heightmap1 = GenImagePerlinNoise(MAP_WIDTH, MAP_HEIGHT, 50, 50, MAP_FIRST_PERLIN_SCALE);
  Image heightmap2 = GenImagePerlinNoise(MAP_WIDTH, MAP_HEIGHT, 100, 100, MAP_SECOND_PERLIN_SCALE);

  Color *pixels1 = LoadImageColors(heightmap1);
  Color *pixels2 = LoadImageColors(heightmap2);

  Image heightmap = GenImageColor(MAP_WIDTH, MAP_HEIGHT, BLACK);
  Image color = GenImageColor(MAP_WIDTH, MAP_HEIGHT, BLACK);

  for (int y = 0; y < heightmap.height; y++) {
    for (int x = 0; x < heightmap.width; x++) {
      int index = (y * heightmap.width) + x;
      
      float noise_value1 = (float)pixels1[index].r;
      float noise_value2 = (float)pixels2[index].r;

      unsigned char combinedHeight = (unsigned char)fminf(255.0f, noise_value1 + noise_value2);

      Color newColor = {
	combinedHeight,
	combinedHeight,
	combinedHeight,
	255
      };

      ImageDrawPixel(&color, x, y, calculate_color_by_height(noise_value1 + noise_value2));
      
      ImageDrawPixel(&heightmap, x, y, newColor);
    }
  }

  UnloadImageColors(pixels1);
  UnloadImageColors(pixels2);

  UnloadImage(heightmap1);
  UnloadImage(heightmap2);

  ImageBlurGaussian(&heightmap, 3);
  
  Mesh mesh = GenMeshHeightmap(heightmap, (Vector3){MAP_MESH_WIDTH, 20, MAP_MESH_HEIGHT});
  s->landscape = LoadModelFromMesh(mesh);

  Texture2D texture = LoadTextureFromImage(heightmap);
  UnloadImage(heightmap);

  Texture2D color_texture = LoadTextureFromImage(color);
  UnloadImage(color);

  s->landscape.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = color_texture;
}

void plug_update(void *state) {
  State *s = (State*)state;
  UpdateCamera(&s->camera, CAMERA_ORBITAL); 
  // UpdateCamera(&s->camera, CAMERA_FREE); 
}

void plug_draw(void *state) {
  State *s = (State*)state;
  BeginDrawing();
  ClearBackground(s->background);

  BeginMode3D(s->camera);

  DrawModel(s->landscape, (Vector3){-MAP_MESH_WIDTH/2, 0, -MAP_MESH_HEIGHT/2 }, 1.f, WHITE);
  DrawGrid(20, 10.0f);

  EndMode3D();

  DrawFPS(10, 10);
  EndDrawing();
}

void plug_deinit(void *state) { 
  State *s = (State*)state;

  UnloadModel(s->landscape);
}
