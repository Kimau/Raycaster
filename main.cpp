// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top
// of imgui.cpp.

#include "core.h"
#include <deque>

#include "glprog.h"
#include "gltex.h"
#include "math/Cvector2.h"
#include "program.h"

int SCREEN_WIDTH = 900;
int SCREEN_HEIGHT = 600;
int SCREEN_FPS = 30;
int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const char *SCREEN_TITLE = "Raycaster - Brute Force - GL";

void AppLoop(SDL_Window *window) {

  // Main loop
  uint32_t lastTick = SDL_GetTicks() + SCREEN_TICKS_PER_FRAME;
  printf("\n------------- LOOOP STARTED --------------- %d \n", lastTick);

  while (true) {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
      if (handleInput(event))
        return;
    }

    ImGui_ImplSdlGL3_NewFrame(window);

    //

    update();

    // Rendering
    render(window);

    // If frame finished early
    uint32_t newTick = SDL_GetTicks();
    if (newTick < lastTick) {
      // Wait remaining time
      SDL_Delay(lastTick - newTick);
    }

    lastTick = SDL_GetTicks() + SCREEN_TICKS_PER_FRAME;
  }
}

bool glinit(); // glrender.cpp

// Main
int main(int, char **) {
  FILE *my_stdout;
  freopen_s(&my_stdout, "my_stdout.txt", "wt", stdout);

  // Setup SDL
  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0) {
    printf("Error: %s\n", SDL_GetError());
    return -1;
  }

  // Setup window
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS,
                      SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
  SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
  SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
  SDL_DisplayMode current;
  SDL_GetCurrentDisplayMode(0, &current);
  SDL_Window *window = SDL_CreateWindow(
      SCREEN_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
      SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL); // SDL_WINDOW_BORDERLESS
  SDL_GLContext glcontext = SDL_GL_CreateContext(window);
  gl3wInit();

  // Setup ImGui binding
  ImGui_ImplSdlGL3_Init(window);

  int versionMagor, versionMinor;
  glGetIntegerv(GL_MAJOR_VERSION, &versionMagor);
  glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
  printf("Version %d.%d - %s\n", versionMagor, versionMinor,
         glGetString(GL_VENDOR));

  // Load Fonts
  // (there is a default font, this is only if you want to change it. see
  // extra_fonts/README.txt for more details)
  // ImGuiIO& io = ImGui::GetIO();
  // io.Fonts->AddFontDefault();
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf",
  // 15.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
  // io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
  // io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f,
  // NULL, io.Fonts->GetGlyphRangesJapanese());

  if (false == glinit())
    goto Label_Shutdown;

  AppLoop(window);

Label_Shutdown:

  // Cleanup
  ImGui_ImplSdlGL3_Shutdown();
  SDL_GL_DeleteContext(glcontext);
  SDL_DestroyWindow(window);
  SDL_Quit();

  return 0;
}
