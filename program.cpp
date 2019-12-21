#include "program.h"

#include <time.h>

bool g_imguiShowTestWindow = false;
bool g_always_cast = false;
bool g_request_brute_ray = false;
bool g_request_save_file = false;
ImVec4 g_clearColour = ImColor(114, 144, 154);
vec2 g_lastclick = vec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
vec3 g_camdir = vec3(0.0f, 0.0f, -1.0f);
vec3 g_campos = vec3(0.0f, 0.0f, 0.0f);
vec2 g_ray_scale = vec2(SCREEN_WIDTH, SCREEN_HEIGHT);

u64 g_walltime = time(NULL);
u64 g_starttime = time(NULL);

float g_drawtest[4][4] = {{2.0f / SCREEN_WIDTH, 0, 0, 0},
                          {0.0f, 2.0f / SCREEN_HEIGHT, 0, 0},
                          {0, 0, -1.0f, 0},
                          {0, 0, 0, 1.0f}};

// Process Events
bool handleInput(SDL_Event event) {

  if (ImGui_ImplSdlGL3_ProcessEvent(&event))
    return false;

  switch (event.type) {
  case SDL_MOUSEMOTION:
    if (event.motion.state & SDL_BUTTON_LMASK) {
      g_lastclick = vec2(event.motion.x, event.motion.y);
    }
    break;

  case SDL_MOUSEBUTTONDOWN:
    break;

  case SDL_MOUSEBUTTONUP:
    break;

  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
    case SDLK_ESCAPE:
      return true;
    }
    break;

  case SDL_QUIT:
    return true;

  default:
    break;
  }

  return false;
}

void update() {
	g_walltime = time(NULL);

  if (ImGui::Begin("Debug Tools")) {
    ImGui::Text("Window Hover %d", ImGui::IsMouseHoveringAnyWindow());
    ImGui::ColorEdit3("clear color", (float *)&g_clearColour);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

	ImGui::DragFloat3("camdir", &g_camdir.x, 0.1f);
	ImGui::DragFloat3("campos", &g_campos.x, 0.1f);

    if (ImGui::BeginMenu("Test Matrix")) {
      if (ImGui::Button("Reset")) {
        g_drawtest[0][0] = 2.0f / SCREEN_WIDTH;
        g_drawtest[0][1] = 0;
        g_drawtest[0][2] = 0;
        g_drawtest[0][3] = 0;
        g_drawtest[1][0] = 0;
        g_drawtest[1][1] = 2.0f / SCREEN_HEIGHT;
        g_drawtest[1][2] = 0;
        g_drawtest[1][3] = 0;
        g_drawtest[2][0] = 0;
        g_drawtest[2][1] = 0;
        g_drawtest[2][2] = -1.0f;
        g_drawtest[2][3] = 0;
        g_drawtest[3][0] = 0;
        g_drawtest[3][1] = 0;
        g_drawtest[3][2] = 0;
        g_drawtest[3][3] = 1.0f;
      }

      ImGui::DragFloat4("_1", &g_drawtest[0][0], 0.01f);
      ImGui::DragFloat4("_2", &g_drawtest[1][0], 0.01f);
      ImGui::DragFloat4("_3", &g_drawtest[2][0], 0.01f);
      ImGui::DragFloat4("_4", &g_drawtest[3][0], 0.01f);

      ImGui::EndMenu();
    }

	ImGui::SliderFloat2("Ray Scale", &g_ray_scale.x, 64.0f, 1024.0f, "%.0f");

	ImGui::Checkbox("always_cast", &g_always_cast);
	if(g_always_cast) g_request_brute_ray = true;

    if ((g_request_brute_ray == false) &&
        (ImGui::Button("Brute Force Raycast")))
      g_request_brute_ray = true;

	if (ImGui::Button("Save File"))
		g_request_save_file = true;
  }
  ImGui::End();
}
