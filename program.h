#pragma once

#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include <GL/gl3w.h>

// Viewport Stuff
bool setupOrthViewport(int width, int height);

void update();
bool handleInput(SDL_Event event);
void render(SDL_Window *window);

//
void Raycast(ImageData &output, int pass_break = 1, bool border = true);

//
extern bool g_imguiShowTestWindow;
extern bool g_request_brute_ray;
extern bool g_always_cast;
extern bool g_request_save_file;
extern ImVec4 g_clearColour;
extern vec3 g_camdir;
extern vec3 g_campos;
extern vec2 g_lastclick;
extern vec2 g_ray_scale;
extern float g_drawtest[4][4];

extern u64 g_walltime;
extern u64 g_starttime;