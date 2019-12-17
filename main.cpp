// ImGui - standalone example application for SDL2 + OpenGL
// If you are new to ImGui, see examples/README.txt and documentation at the top of imgui.cpp.

#include <imgui.h>
#include "imgui_impl_sdl_gl3.h"
#include <stdio.h>
#include <GL/gl3w.h>    // This example is using gl3w to access OpenGL functions (because it is small). You may use glew/glad/glLoadGen/etc. whatever already works for you.
#include "libs/sdl/include/SDL.h"

#include <deque>

#include "math/Cvector2.h"
#include "program.h"
#include "gltex.h"
#include "glprog.h"
#include "net.h"
#include "text_help.h"

const int SCREEN_WIDTH = 900;
const int SCREEN_HEIGHT = 600;
const int SCREEN_FPS = 30;
const int SCREEN_TICKS_PER_FRAME = 1000 / SCREEN_FPS;
const char* SCREEN_TITLE = "ImGui SDL2+OpenGL3 example";

// Globals
bool g_imguiShowTestWindow = false;
ImVec4 g_clearColour = ImColor(114, 144, 154);

GLuint g_testVertexArrays;
GLuint g_testTexVertexArrays;

ImageData g_imgTest = ImageData("test");

Vec2 LastClick = Vec2(SCREEN_WIDTH*0.5f, SCREEN_HEIGHT*0.5f);

// #define NET_ENABLE 1

/////////////////////

bool loadBasicColorProgram()
{
	ProgramData* prog = ProgramData::GetProg(PD_BASIC_RED);

	const GLchar *vertexSource =
		"#version 330\n"
		"in vec4 Position;\n"
		"in vec4 Color;\n"
		"out vec4 Frag_Color;\n"
		"uniform mat4 ProjMtx;\n"
		"void main()\n"
		"{\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * Position;\n"
		"}\n";

	const GLchar* fragmentSource =
		"#version 330\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color; \n"
		"}\n";

	prog->create();
	prog->compileAttachShader(vertexSource, GL_VERTEX_SHADER);
	prog->compileAttachShader(fragmentSource, GL_FRAGMENT_SHADER);
	prog->setupAttrib("Position");
	prog->setupAttrib("Color");
	prog->setupUniform("ProjMtx");
	if (prog->link() == false)
		return false;

	// Setup Arrays
	GLfloat testVertData[4 * 3] = {
		25.0f, 25.0f, 0.0f, 1.0f,
		-25.0f, 25.0f, 0.0f, 1.0f,
		0.0f, -25.0f, 0.0f, 1.0f };
	GLfloat testColData[4 * 3] = {
		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,
	};

	glGenVertexArrays(1, &g_testVertexArrays);
	glBindVertexArray(g_testVertexArrays);

	genBasicFloatNBuffer(0, prog->attribList[0], testVertData, 3, 4);
	genBasicFloatNBuffer(1, prog->attribList[1], testColData, 3, 4);

	glBindVertexArray(0);
							  
	return true;
}


bool loadBasicTexProgram()
{
	ProgramData* prog = ProgramData::GetProg(PD_BASIC_TEX);
	
	const GLchar *vertexSource =
		"#version 330\n"
		"in vec4 Position;\n"
		"in vec4 Color;\n"
		"in vec2 TexCoords;\n"
		"out vec4 Frag_Color;\n"
		"out vec2 Tex_UV;\n"
		"uniform mat4 ProjMtx;\n"
		"void main()\n"
		"{\n"
		"	Frag_Color = Color;\n"
		"	Tex_UV = TexCoords;\n"
		"	gl_Position = ProjMtx * Position;\n"
		"}\n";

	const GLchar* fragmentSource =
		"#version 330\n"
		"uniform sampler2D LTextureUnit;\n"
		"in vec4 Frag_Color;\n"
		"in vec2 Tex_UV;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = texture( LTextureUnit, Tex_UV ) * Frag_Color; \n"
		"}\n";


	prog->create();
	prog->compileAttachShader(vertexSource, GL_VERTEX_SHADER);
	prog->compileAttachShader(fragmentSource, GL_FRAGMENT_SHADER);
	prog->setupAttrib("Position");
	prog->setupAttrib("Color");
	prog->setupAttrib("TexCoords");
	prog->setupUniform("ProjMtx");
	prog->setupUniform("LTextureUnit");
	if (prog->link() == false)
		return false;

	// Setup Arrays
	const GLuint numVerts = 8;
	GLfloat testVertData[numVerts * 4] = {
		150.0f, 100.0f, 0.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f,
		100.0f, 100.0f, 0.0f, 1.0f,

		150.0f, 100.0f, 0.0f, 1.0f,
		100.0f, 200.0f, 0.0f, 1.0f,
		150.0f, 200.0f, 0.0f, 1.0f
	};
	GLfloat testColData[numVerts * 4] = {
		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		0.0f,0.0f,1.0f,1.0f,

		1.0f,0.0f,0.0f,1.0f,
		0.0f,1.0f,0.0f,1.0f,
		1.0f,1.0f,1.0f,1.0f,
	};
	GLfloat testTexData[numVerts * 2] = {
		1.0f,0.0f,
		0.0f,1.0f,
		0.0f,0.0f,

		1.0f,0.0f,
		0.0f,1.0f,
		1.0f,1.0f
	};


	glGenVertexArrays(1, &g_testTexVertexArrays);
	glBindVertexArray(g_testTexVertexArrays);

	genBasicFloatNBuffer(0, prog->attribList[0], testVertData, numVerts, 4);
	genBasicFloatNBuffer(1, prog->attribList[1], testColData, numVerts, 4);
	genBasicFloatNBuffer(2, prog->attribList[2], testTexData, numVerts, 2);

	glBindVertexArray(0);
	return true;
}
/////////////////////


// Render
void render(SDL_Window* window) {
	int width = SCREEN_WIDTH;
	int height = SCREEN_HEIGHT;
	float invWidth = 1.0f / float(width);
	float invHeight = 1.0f / float(height);

	// Setup Render State
	glViewport(0, 0, width, height);

	// Clear
	glClearColor(g_clearColour.x, g_clearColour.y, g_clearColour.z, g_clearColour.w);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled
	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);

	// Setup viewport, orthographic projection matrix
	const float ortho_projection[4][4] =
	{
		{ 2.0f *invWidth, 0, 0,  0},
		{ 0.0f, 2.0f *-invHeight, 0, 0 },
		{ 0,0, -1.0f, 0 },
		{ -1.0f + LastClick.x*2.0f*invWidth, 1.0f + LastClick.y*2.0f*-invHeight, 0, 1.0f },
	};


	{
		ProgramData* prog = ProgramData::GetProg(PD_BASIC_TEX);

		prog->bind();
		prog->uniformList[0].SetMatrix4(&(ortho_projection[0][0]));
		glBindVertexArray(g_testVertexArrays);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glDrawArrays(GL_TRIANGLES, 0, 3);
	}

	{
		ProgramData* prog = ProgramData::GetProg(PD_BASIC_TEX);

		prog->bind();
		prog->uniformList[0].SetMatrix4(&(ortho_projection[0][0]));
		glActiveTexture(GL_TEXTURE0 + prog->uniformList[1].loc);
		g_imgTest.BindTex();

		glBindVertexArray(g_testTexVertexArrays);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glDrawArrays(GL_TRIANGLES, 0, 6);
	}

	// Clean Up
	glBindVertexArray(NULL);
	glUseProgram(NULL);

	//
	ImGui::Render();
	SDL_GL_SwapWindow(window);
}

// Process Events
bool handleInput(SDL_Event event) {

	if (ImGui_ImplSdlGL3_ProcessEvent(&event))
		return false;

	switch (event.type)
	{
	case SDL_MOUSEMOTION:
		if (event.motion.state & SDL_BUTTON_LMASK) {
			LastClick = Vec2(event.motion.x, event.motion.y);
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

int g_TimeSincePing = 0;
std::deque<std::string> g_msgQ = std::deque<std::string>();


bool handleMessage(const char* message) {
	if (strcmp(message, "PING") == 0) {
		g_TimeSincePing = 0;
		return true;
	}

	if (message[0] == '{') {
		// TODO :: JSON Processing in Future
		int catNum;
		const char* searchStr = "\"cat\"";

		if (ExtractNumber(message, searchStr, catNum) == false)
			return false;


			switch (catNum) {
			case 35:
			{
				auto nick = ExtractString(message, "\"nick\"");
				auto msg = ExtractString(message, "\"content\"");
				TrimString(nick);
				TrimString(msg);

				g_msgQ.push_back(nick + ": " + msg);
				return true;
			}

			default:
				return false;
			}
	}

	return false;
}


void AppLoop(SDL_Window * window)
{
#ifdef NET_ENABLE
	// Connect to Network
	auto netThread = run_client("127.0.0.1", 39875);
	char buffer[1000];
#endif

	// Main loop
	uint32_t lastTick = SDL_GetTicks() + SCREEN_TICKS_PER_FRAME;
	printf("\n------------- LOOOP STARTED --------------- %d \n", lastTick);

	int timeSincePop = 0;

	while (true)
	{
		SDL_Event event;
		while (SDL_PollEvent(&event))
		{
			if (handleInput(event))
				return;
		}
#ifdef NET_ENABLE
		while (getMessage(netThread, buffer, 1000)) {
			if (handleMessage(buffer) == false) {
				g_msgQ.push_back(buffer);
			}
		}
#endif
		ImGui_ImplSdlGL3_NewFrame(window);

		//
		g_TimeSincePing++;
		timeSincePop++;

		// Pop Stale Messages
		for (auto sz = g_msgQ.size(); (sz > 0) && ((sz > 10) || (timeSincePop > 200)); --sz) {
			g_msgQ.pop_front();
			timeSincePop = 0;
		}


		// 1. Show a simple window
		// Tip: if we don't call ImGui::Begin()/ImGui::End() the widgets appears in a window automatically called "Debug"
		{
			ImGui::Text("Window Hover %d", ImGui::IsMouseHoveringAnyWindow());
			ImGui::ColorEdit3("clear color", (float*)&g_clearColour);
			if (ImGui::Button("Test Window")) g_imguiShowTestWindow ^= 1;
			ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
		}

#ifdef NET_ENABLE
		ImGui::Begin("Twitch Message");
		{
			ImGui::Text("Ping %d", g_TimeSincePing);
			for (const auto& line : g_msgQ) {
				ImGui::Text("%s", line.c_str());
			}
		}
		ImGui::End();
#endif

		// 3. Show the ImGui test window. Most of the sample code is in ImGui::ShowTestWindow()
		if (g_imguiShowTestWindow)
		{
			ImGui::SetNextWindowPos(ImVec2(650, 20), ImGuiSetCond_FirstUseEver);
			ImGui::ShowTestWindow(&g_imguiShowTestWindow);
		}

		// Rendering
		render(window);

		// If frame finished early
		uint32_t  newTick = SDL_GetTicks();
		if (newTick < lastTick)
		{
			//Wait remaining time
			SDL_Delay(lastTick - newTick);
		}

		lastTick = SDL_GetTicks() + SCREEN_TICKS_PER_FRAME;
	}
}

// Main
int main(int, char**)
{
	FILE* my_stdout;
	freopen_s(&my_stdout, "my_stdout.txt", "wt", stdout);
	
	// Setup SDL
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) != 0)
	{
		printf("Error: %s\n", SDL_GetError());
		return -1;
	}

	// Setup window
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_FORWARD_COMPATIBLE_FLAG);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
	SDL_DisplayMode current;
	SDL_GetCurrentDisplayMode(0, &current);
	SDL_Window *window = SDL_CreateWindow(SCREEN_TITLE, SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_OPENGL); // SDL_WINDOW_BORDERLESS
	SDL_GLContext glcontext = SDL_GL_CreateContext(window);
	gl3wInit();

	// Setup ImGui binding
	ImGui_ImplSdlGL3_Init(window);

	int versionMagor, versionMinor;
	glGetIntegerv(GL_MAJOR_VERSION, &versionMagor);
	glGetIntegerv(GL_MINOR_VERSION, &versionMinor);
	printf("Version %d.%d - %s\n", versionMagor, versionMinor, glGetString(GL_VENDOR));

	// Load Fonts
	// (there is a default font, this is only if you want to change it. see extra_fonts/README.txt for more details)
	//ImGuiIO& io = ImGui::GetIO();
	//io.Fonts->AddFontDefault();
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/Cousine-Regular.ttf", 15.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/DroidSans.ttf", 16.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyClean.ttf", 13.0f);
	//io.Fonts->AddFontFromFileTTF("../../extra_fonts/ProggyTiny.ttf", 10.0f);
	//io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());

	if (loadBasicColorProgram() == false) {
		printf("\n\nBasic Colour Prog\nError %08X\n", glGetError());
		goto Label_Shutdown;
	}

	if (loadBasicTexProgram() == false) {
		printf("\n\nBasic Tex Prog\nError %08X\n", glGetError());
		goto Label_Shutdown;
	}


	printf("GL CHECK Error %08X\n", glGetError());

	g_imgTest.LoadFromFile("./img/test.png");
	if (g_imgTest.CopyToGPU() == false) {
		goto Label_Shutdown;
	}

	// Check for GL Errors
	int x = glGetError();
	if (x != GL_NO_ERROR) {
		printf("\nError %08X\n", x);
		goto Label_Shutdown;
	}

	AppLoop(window);

Label_Shutdown:


	// Cleanup
	ImGui_ImplSdlGL3_Shutdown();
	SDL_GL_DeleteContext(glcontext);
	SDL_DestroyWindow(window);
	SDL_Quit();

	return 0;
}
