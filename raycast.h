#pragma once

#include "math/algebra.h"

enum debug_mode
{
	NO_DEBUG,
	DEBUG_NORMAL,
	DEBUG_BOUNCE_DIR,
	DEBUG_TIME,
	NOOF_DEBUG_MODES
};

enum draw_mode
{
	NO_DRAW,
	DRAW_FULL_FRAME,
	DRAW_ONE_LINE,
	DRAW_RANDOM,
	NOOF_DRAW_MODES
};

//
struct raycast_state {
	int num_passes = 1;
	int num_samples = 3;
	int num_bounces = 30;
	bool border = true;
	bool want_draw = false;
	bool use_poisson = true;
	bool use_recurse = false;
	debug_mode debug = NO_DEBUG;
	draw_mode drawing = NO_DRAW;

	ray3 cam = ray3(vec3(0.0f, 0.0f, 1.5f), vec3(0.0f, 1.0f, 0.0f));
	vec3 up = vec3(0.0f, 0.0f, 1.0f);
	vec2 ray_scale = vec2(SCREEN_WIDTH * 0.4f, SCREEN_HEIGHT * 0.4f);
	int curr_pass = -1;
};
