#include "program.h"

#include <time.h>
#include <vector>
#include "world.h"
#include "material.h"

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
		30, // int num_passes = 1;
		1, // int num_samples = 3;
		50, // int num_bounces = 30;
		false, // bool border = true;
		true, // bool want_draw = false;
		true, // bool use_poisson = true;
		false, // bool use_recurse = false;
		NO_DEBUG, // debug_mode dmode = NO_DEBUG;
		DRAW_RANDOM // draw_mode draw_mode = NO_DRAW;
	},

{
	30, // int num_passes = 1;
	50, // int num_samples = 3;
	1000, // int num_bounces = 30;
	true, // bool border = true;
	false, // bool want_draw = false;
	true, // bool use_poisson = true;
	false, // bool use_recurse = false;
	NO_DEBUG, // debug_mode dmode = NO_DEBUG;
	DRAW_ONE_LINE // draw_mode draw_mode = NO_DRAW;
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
		  case SDLK_a: rs.cam.a -= cross(rs.cam.b, up_vec) * 0.1f; break;
		  case SDLK_d: rs.cam.a += cross(rs.cam.b, up_vec) * 0.1f; break;
		  case SDLK_q: rs.cam.a += up_vec * 0.1f; break;
		  case SDLK_e: rs.cam.a -= up_vec * 0.1f; break;

		  case SDLK_LEFT:  rs.cam.b = rs.cam.b.RotateZ(-5.0f); break;
		  case SDLK_RIGHT: rs.cam.b = rs.cam.b.RotateZ(+5.0f); break;
		  case SDLK_UP:    rs.cam.b = rs.cam.b.Rotate(-5.0f, cross(rs.cam.b, up_vec)); break;
		  case SDLK_DOWN:  rs.cam.b = rs.cam.b.Rotate(+5.0f, cross(rs.cam.b, up_vec)); break;
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
	std::vector<solidhit*> vsolids;
	std::vector<material*> vmats;

	vmats.push_back(new mat_skysphere({
		vec4(0.2f, 0.2f, 0.2f,-0.9f),
		vec4(1.0f, 0.8f, 0.8f,-0.3f),
		vec4(1.0f, 1.0f, 1.0f, 0.0f),
		vec4(0.5f, 0.7f, 1.0f, 0.7f),
		vec4(0.7f, 0.7f, 1.0f, 0.9f) }));
	vsolids.push_back(new solid_skysphere(1000.0f));
	vsolids.back()->mat = u16(vmats.size() - 1);

	vmats.push_back(new mat_tiles(vec3(1.0f, 0.8f, 0.8f), vec3(0.8f, 1.0f, 0.8f), 0.2f));
	vsolids.push_back(new solid_plane(vec4(0.0f, 0.0f, 1.0f, 0.0f)));
	vsolids.back()->mat = u16(vmats.size() - 1);

	vmats.push_back(new mat_lambertian(vec3(1.0f, 1.0f, 1.0f), 0.0f));
	for (vec4 p : {
		vec4(-3.0f, 10.0f, 2.0f, 0.5f),
			vec4(0.0f, 10.0f, 1.0f, 0.5f),
			vec4(+2.7f, 10.5f, 0.7f, 0.5f),
			vec4(+2.7f, 10.5f, 1.7f, 0.5f),
			vec4(+4.0f, 10.0f, 1.0f, 0.9f)
	}) {
		vsolids.push_back(new solid_ball(p));
		vsolids.back()->mat = u16(vmats.size() - 1);
	}

	vmats.push_back(new mat_normal());
	vsolids[3]->mat = u16(vmats.size() - 1);

	vmats.push_back(new mat_metal(vec3(1.0f, 1.0f, 1.0f), 0.0f));
	vsolids[4]->mat = u16(vmats.size() - 1);
	vsolids[5]->mat = u16(vmats.size() - 1);

	vsolids.push_back(new solid_ball(vec4(-5.0f, 5.0f, 3.0f, 2.0f)));
	vmats.push_back(new mat_lambertian(vec3(1.0f, 0.0f, 0.0f), 0.5f));
	vsolids.back()->mat = u16(vmats.size() - 1);

	//vsolids.push_back(new solid_ball(vec4(2.0f, 2.0f, 0.0f, 2.0f)));
	//vmats.push_back(new mat_dielectric(RefIdxGlass));
	//vsolids.back()->mat = u16(vmats.size() - 1);

	//
	world& w = getModifyWorld();
	w.free_world();

	w.num_solids = vsolids.size();
	w.list_solids = new solidhit*[w.num_solids];
	for (u64 i = 0; i < w.num_solids; ++i)
		w.list_solids[i] = vsolids[i];

	w.num_materials = u16(vmats.size());
	w.list_materials = new material*[w.num_materials];
	for (u16 i = 0; i < w.num_materials; ++i)
		w.list_materials[i] = vmats[i];
}


void update_imgui()
{

	//////////////////////////////////////////////////////////////////////////
	// ImGUI
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
			if (ImGui::BeginMenu("Render Mode")) {
				const char* names[NOOF_RENDER_STATES] = {
					"Always",
					"Single Shot"
				};

				RenderType old_state = g_render_state_index;
				if (ImGui::ListBox("", (int*)&g_render_state_index, names, NOOF_RENDER_STATES)) {
					g_render_states[g_render_state_index].cam = g_render_states[old_state].cam; // Copy from old state
				}

				ImGui::EndMenu();
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

			ImGui::Separator();

			{
				{
					const char* labels[NOOF_DRAW_MODES] = {
						"NO_DRAW",
						"DRAW_FULL_FRAME",
						"DRAW_ONE_LINE",
						"DRAW_RANDOM",
					};

					if (ImGui::BeginMenu("Drawing")) {
						ImGui::ListBox("", (int*)&rs.drawing, labels, NOOF_DEBUG_MODES);
						ImGui::EndMenu();
					}
					ImGui::Text(labels[rs.drawing]);
				}

				{
					const char* labels[NOOF_DEBUG_MODES] = {
						"NO_DEBUG",
						"DEBUG_NORMAL",
						"DEBUG_BOUNCE_DIR",
						"DEBUG_TIME",
					};

					if (ImGui::BeginMenu("Debug")) {
						ImGui::ListBox("", (int*)&rs.debug, labels, NOOF_DEBUG_MODES);
						ImGui::EndMenu();
					}
					ImGui::Text(labels[rs.debug]);
				}

				ImGui::DragFloat3("Pos", &rs.cam.a.x, 0.1f);
				ImGui::DragFloat3("Dir", &rs.cam.b.x, 0.1f);
				ImGui::SliderFloat2("Scale", &rs.ray_scale.x, 64.0f, 1024.0f, "%.0f");
				ImGui::Checkbox("Poisson Sampling", &rs.use_poisson);
				ImGui::Checkbox("Recurse", &rs.use_recurse);
				ImGui::InputInt("Num of Passes", &rs.num_passes);
				ImGui::InputInt("Samples", &rs.num_samples);
				ImGui::InputInt("Bounces", &rs.num_bounces);
			}

			if (false == rs.want_draw)
				rs.want_draw = ImGui::Button("Redraw");
		}
		ImGui::Separator();

		if (ImGui::Button("Save File"))
			g_request_save_file = true;
	}
	ImGui::End();
}

void update() {
	time((time_t*)&g_walltime);
	g_ticks_since_start = SDL_GetTicks();

	//
	world& w = getModifyWorld();
	((solid_ball*)(w.list_solids[2]))->v.w = 0.25f + (sinf(g_ticks_since_start * 0.001f) + 1.0f) * 1.0f;
	((solid_ball*)(w.list_solids[3]))->v.z = sinf(g_ticks_since_start * 0.001f) * 2.0f;

	update_imgui();
}
