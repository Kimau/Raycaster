#include "program.h"

#include <time.h>

bool g_imguiShowTestWindow = false;
bool g_request_save_file = false;
ImVec4 g_clearColour = ImColor(114, 144, 154);
vec2 g_lastclick = vec2(SCREEN_WIDTH * 0.5f, SCREEN_HEIGHT * 0.5f);

u64 g_walltime = 0;
u64 g_starttime = 0;
u64 g_ticks_since_start = 0;

RenderType g_render_state_index = RS_ALWAYS;
raycast_state g_render_states[NOOF_RENDER_STATES] = {
	{
		10,
		5,
		8,
		false,
		false
	},

{
	0,
	50,
	1000,
	true,
	false
}
};

float g_drawtest[4][4] = {{2.0f / SCREEN_WIDTH, 0, 0, 0},
                          {0.0f, 2.0f / SCREEN_HEIGHT, 0, 0},
                          {0, 0, -1.0f, 0},
                          {0, 0, 0, 1.0f}};

// Process Events
bool handleInput(SDL_Event event) {
	raycast_state& rs = g_render_states[g_render_state_index];

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

  case SDL_KEYDOWN:
	  switch (event.key.keysym.sym) {
		  case SDLK_w: rs.cam.a += rs.cam.b * 0.1f; break;
		  case SDLK_s: rs.cam.a -= rs.cam.b * 0.1f; break;
		  case SDLK_a: rs.cam.a -= rs.cam.b * up_vec * 0.1f; break;
		  case SDLK_d: rs.cam.a += rs.cam.b * up_vec * 0.1f; break;
		  case SDLK_q: rs.cam.a += up_vec * 0.1f; break;
		  case SDLK_e: rs.cam.a -= up_vec * 0.1f; break;

		  case SDLK_LEFT:  rs.cam.b = rs.cam.b.RotateZ(-5.0f); break;
		  case SDLK_RIGHT: rs.cam.b = rs.cam.b.RotateZ(+5.0f); break;
		  case SDLK_UP:    rs.cam.b = rs.cam.b.Rotate(-5.0f, rs.cam.b * up_vec); break;
		  case SDLK_DOWN:  rs.cam.b = rs.cam.b.Rotate(+5.0f, rs.cam.b * up_vec); break;
	  } break;

  case SDL_KEYUP:
    switch (event.key.keysym.sym) {
		case SDLK_ESCAPE: return true;
    } break;

  case SDL_QUIT:
    return true;

  default:
    break;
  }

  return false;
}

void init() {

}

void update() {
	time((time_t*)&g_walltime);
	g_ticks_since_start = SDL_GetTicks();

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
	ImGui::Separator();
	{

		const char* names[2] = {
			"Always",
			"Single Shot"
		};

		RenderType old_state = g_render_state_index;
		if (ImGui::ListBox("", (int*)&g_render_state_index, names, NOOF_RENDER_STATES)) {
			g_render_states[g_render_state_index].cam = g_render_states[old_state].cam; // Copy from old state
		}

		raycast_state& rs = g_render_states[g_render_state_index];
		switch (g_render_state_index) {
		case RS_ALWAYS:
			rs.want_draw = true;
			break;
		case RS_SINGLE_SHOT:
			rs.want_draw = false;
			break;
		}

		{
			if (false == rs.want_draw)
				rs.want_draw = ImGui::Button("Redraw");
			ImGui::DragFloat3("Pos", &rs.cam.a.x, 0.1f);
			ImGui::DragFloat3("Dir", &rs.cam.b.x, 0.1f);
			ImGui::SliderFloat2("Scale", &rs.ray_scale.x, 64.0f, 1024.0f, "%.0f");
			ImGui::Checkbox("Poisson Sampling", &rs.use_poisson);
			ImGui::InputInt("Samples", &rs.num_samples);
			ImGui::InputInt("Bounces", &rs.num_bounces);
		}
	}
	ImGui::Separator();

	if (ImGui::Button("Save File"))
		g_request_save_file = true;
  }
  ImGui::End();
}
