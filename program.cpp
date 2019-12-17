#include "program.h"

bool g_imguiShowTestWindow = false;
bool g_request_brute_ray = false;
ImVec4 g_clearColour = ImColor(114, 144, 154);
Vec2 g_lastclick = Vec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);
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
      g_lastclick = Vec2(event.motion.x, event.motion.y);
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
  if (ImGui::Begin("Debug Tools")) {
    ImGui::Text("Window Hover %d", ImGui::IsMouseHoveringAnyWindow());
    ImGui::ColorEdit3("clear color", (float *)&g_clearColour);
    ImGui::Text("Application average %.3f ms/frame (%.1f FPS)",
                1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

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

    if ((g_request_brute_ray == false) &&
        (ImGui::Button("Brute Force Raycast")))
      g_request_brute_ray = true;
  }
  ImGui::End();
}
