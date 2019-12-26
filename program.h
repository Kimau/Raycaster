#pragma once

#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include <GL/gl3w.h>

// Viewport Stuff
bool setupOrthViewport(int width, int height);

void init();
void update();
bool handleInput(SDL_Event event);
void render(SDL_Window *window);

//
struct raycast_state {
	int pass_break = 1;
	int num_samples = 3;
	bool border = true;
	bool want_draw = false;

	ray3 cam = ray3(vec3(0.0f, 0.0f, 1.5f), vec3(0.0f, 1.0f, 0.0f));
	vec3 up = vec3(0.0f, 0.0f, 1.0f);
	vec2 ray_scale = vec2(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.4f);
};

void Raycast(ImageData &output, const raycast_state& state);

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
