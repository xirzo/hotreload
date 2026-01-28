#include "plug.h"

#define RAYMATH_IMPLEMENTATION

#include <math.h>
#include <raymath.h>
#include <raylib.h>
#include <stdlib.h>


// TODO: PROVIDE SCREEN WIDTH AND HEIGHT FROM MAIN
#define SCREEN_WIDTH 900
#define SCREEN_HEIGHT 900

typedef struct State {
    Color background;
} State;

size_t plug_state_size(void) {
    return sizeof(State);
}

void plug_init(void *state) { 
    State *s = (State*)state;
    s->background = BLACK;
}

void draw_point(Vector2 coords, Color color) {
  static const int point_side = 20;
  int half_point_side = point_side / 2;

  DrawRectangle(coords.x - half_point_side,
		coords.y - half_point_side,
		point_side,
		point_side,
		color);
}

void draw_line(Vector2 p1, Vector2 p2, Color color) {
  DrawLine(p1.x, p1.y, p2.x, p2.y, color);
}


Vector2 screen(Vector2 coords) {
  return (Vector2){
    (coords.x + 1) / 2 * SCREEN_WIDTH,
    (1 - (coords.y + 1) / 2) * SCREEN_HEIGHT
  };
}

Vector2 project(Vector3 coords) {
  return (Vector2){
    coords.x / coords.z,
    coords.y / coords.z
  };
}

Vector3 rotate_xy(Vector3 coords, float angle) {
  float c = cosf(angle);
  float s = sinf(angle);

  return (Vector3) {
    coords.x * c - coords.y * s,
    coords.x * s + coords.y * c,
    coords.z
  };
}

Vector3 rotate_xz(Vector3 coords, float angle) {
  float c = cosf(angle);
  float s = sinf(angle);

  return (Vector3) {
    coords.x * c - coords.z * s,
    coords.y,
    coords.x * s + coords.z * c
  };
}

static Vector3 points[] = {
  {-0.25f, -0.25f, 0.25},
  {0.25f, -0.25f, 0.25},
  {-0.25f, 0.25f, 0.25},
  {0.25f, 0.25f, 0.25},

  {-0.25f, -0.25f, -0.25},
  {0.25f, -0.25f, -0.25},
  {-0.25f, 0.25f, -0.25},
  {0.25f, 0.25f, -0.25},
};

static int faces[2][4] = {
  {0, 1, 2, 3},
  {4, 5, 6, 7},
  {0, 4},
  {1, 5},
  {2, 6},
  {3, 7},
};

static float angle = 0.f;
static float object_z = 0.f; 

void plug_update(void *state) {
    State *s = (State*)state;

    angle += 2 * PI * GetFrameTime();
    object_z += 1.0f * GetFrameTime(); 
}

void plug_draw(void *state) {
    State *s = (State*)state;
    BeginDrawing();
    ClearBackground(s->background);

    for (size_t i = 0; i < sizeof(faces)/sizeof(faces[0]); i++) {
	for (size_t j = 0; j < sizeof(faces[i])/sizeof(faces[i][0]); j++) {
	    Vector3 p1 = points[faces[i][j]];
	    Vector3 p2 = points[faces[i][(j+1)%(sizeof(faces)/sizeof(faces[0]))]];

	    p1 = rotate_xz(p1, angle);
	    p2 = rotate_xz(p2, angle);

	    p1.z += 1.0f + object_z; 
	    p2.z += 1.0f + object_z;

	    draw_line(screen(project(p1)), screen(project(p2)), RED);
	}
    }


    EndDrawing();
}

void plug_deinit(void *state) { 
    (void)state;
}
