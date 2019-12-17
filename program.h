#pragma once

#include "core.h"
#include "gltex.h"
#include "math/Cvector2.h"
#include <GL/gl3w.h>

// Viewport Stuff
bool setupOrthViewport(int width, int height);

void update();
bool handleInput(SDL_Event event);
void render(SDL_Window *window);

//
void Raycast(ImageData &output);

//
extern bool g_imguiShowTestWindow;
extern bool g_request_brute_ray;
extern ImVec4 g_clearColour;
extern Vec2 g_lastclick;
extern float g_drawtest[4][4];