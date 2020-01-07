#pragma once

#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include <GL/gl3w.h>
#include "raycast.h"

//
extern bool g_imguiShowTestWindow;
extern bool g_request_save_file;
extern ImVec4 g_clearColour;
extern vec2 g_lastclick;
extern float g_drawtest[4][4];

extern u64 g_walltime;
extern u64 g_starttime;
extern u64 g_ticks_since_start;

enum RenderType
{
	RS_ALWAYS,
	RS_SINGLE_SHOT,
	NOOF_RENDER_STATES
};

extern RenderType g_render_state_index;
extern raycast_state g_render_states[NOOF_RENDER_STATES];

// Viewport Stuff
bool setupOrthViewport(int width, int height);

void init();
void update();
bool handleInput(SDL_Event event);
void render(SDL_Window *window, raycast_state& rs);
