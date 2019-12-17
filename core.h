#pragma once

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include "libs/sdl/include/SDL.h"


extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_FPS;
extern int SCREEN_TICKS_PER_FRAME;
extern const char* SCREEN_TITLE;
