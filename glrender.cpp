#include "core.h"
#include "program.h"

#include "glprog.h"
#include "gltex.h"
#include "math/Cvector2.h"
#include "filesys.h"

#include <stdio.h>

GLuint g_testVertexArrays;
GLuint g_testTexVertexArrays;
ImageData g_imgTest = ImageData("test");
ImageData g_rayresult;

/////////////////////

bool loadTestComputeProgram() {
	ProgramData *prog = ProgramData::GetProg(PD_COMPUTE_TEST);
	std::vector<u8> buf = ReadFileToBuffer("..\\shaders\\compute_test.glsl");

	prog->create();
	prog->compileAttachShader((const char*)&buf[0], GL_COMPUTE_SHADER);
	prog->setupUniform("destTex");
	prog->setupUniform("roll");
	if (prog->link() == false)
		return false;

	glCheckError("Compute shader");
	return true;
}

bool loadBasicColorProgram() {
  ProgramData *prog = ProgramData::GetProg(PD_BASIC_RED);

  std::vector<u8> src_frag = ReadFileToBuffer("..\\shaders\\basic_col_frag.glsl");
  std::vector<u8> src_vert = ReadFileToBuffer("..\\shaders\\basic_col_vert.glsl");

  prog->create();
  prog->compileAttachShader((const char*)&src_vert[0], GL_VERTEX_SHADER);
  prog->compileAttachShader((const char*)&src_frag[0], GL_FRAGMENT_SHADER);
  prog->setupAttrib("Position");
  prog->setupAttrib("Color");
  prog->setupUniform("ProjMtx");
  if (prog->link() == false)
    return false;

  // Setup Arrays
  GLfloat testVertData[4 * 3] = {25.0f, 25.0f, 0.0f, 1.0f,   -25.0f, 25.0f,
                                 0.0f,  1.0f,  0.0f, -25.0f, 0.0f,   1.0f};
  GLfloat testColData[4 * 3] = {
      1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f,
  };

  glGenVertexArrays(1, &g_testVertexArrays);
  glBindVertexArray(g_testVertexArrays);

  genBasicFloatNBuffer(0, prog->attribList[0], testVertData, 3, 4);
  genBasicFloatNBuffer(1, prog->attribList[1], testColData, 3, 4);

  glBindVertexArray(0);

  return true;
}

bool loadBasicTexProgram() {
  ProgramData *prog = ProgramData::GetProg(PD_BASIC_TEX);

  std::vector<u8> src_frag = ReadFileToBuffer("..\\shaders\\basic_tex_frag.glsl");
  std::vector<u8> src_vert = ReadFileToBuffer("..\\shaders\\basic_tex_vert.glsl");

  prog->create();
  prog->compileAttachShader((const char*)&src_vert[0], GL_VERTEX_SHADER);
  prog->compileAttachShader((const char*)&src_frag[0], GL_FRAGMENT_SHADER);

  prog->setupAttrib("Position");
  prog->setupAttrib("Color");
  prog->setupAttrib("TexCoords");
  prog->setupUniform("ProjMtx");
  prog->setupUniform("LTextureUnit");
  if (prog->link() == false)
    return false;

  // Setup Arrays
  const GLuint numVerts = 8;
  GLfloat testVertData[numVerts * 4] = {1.0f, -1.0f, 0.0f,  1.0f,  -1.0f, 1.0f,
                                        0.0f, 1.0f,  -1.0f, -1.0f, 0.0f,  1.0f,

                                        1.0f, -1.0f, 0.0f,  1.0f,  -1.0f, 1.0f,
                                        0.0f, 1.0f,  1.0f,  1.0f,  0.0f,  1.0f};
  /*GLfloat testColData[numVerts * 4] = { Rainbow
          1.0f,0.0f,0.0f,1.0f,
          0.0f,1.0f,0.0f,1.0f,
          0.0f,0.0f,1.0f,1.0f,

          1.0f,0.0f,0.0f,1.0f,
          0.0f,1.0f,0.0f,1.0f,
          1.0f,1.0f,1.0f,1.0f,
  };*/
  GLfloat testColData[numVerts * 4] = {
      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,

      1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f,
  };
  GLfloat testTexData[numVerts * 2] = {1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,

                                       1.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f};

  glGenVertexArrays(1, &g_testTexVertexArrays);
  glBindVertexArray(g_testTexVertexArrays);

  genBasicFloatNBuffer(0, prog->attribList[0], testVertData, numVerts, 4);
  genBasicFloatNBuffer(1, prog->attribList[1], testColData, numVerts, 4);
  genBasicFloatNBuffer(2, prog->attribList[2], testTexData, numVerts, 2);

  glBindVertexArray(0);
  return true;
}

/////////////////////

bool glinit() {
  if (loadBasicColorProgram() == false) {
    DebugLog("\n\nBasic Colour Prog\nError %08X\n", glGetError());
    return false;
  }

  if (loadBasicTexProgram() == false) {
    DebugLog("\n\nBasic Tex Prog\nError %08X\n", glGetError());
    return false;
  }

  if (loadTestComputeProgram() == false) {
	  DebugLog("\n\nTest Compute Program\nError %08X\n", glGetError());
	  return false;
  }

  glCheckError("Init");

  g_imgTest.LoadFromFile("./img/test.png");
  if (g_imgTest.CopyToGPU() == false) {
    return false;
  }

  return glCheckError("CopyGPU");

}

bool setupOrthViewport(int width, int height) { return false; }


void Raycast(ImageData &output, const raycast_state& state);

// Render
void render(SDL_Window *window, raycast_state& rs) {
  // Setup Render State
  glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
  const int ray_width = int(rs.ray_scale.x / 32.0f) * 32;
  const int ray_height = int(rs.ray_scale.y / 32.0f) * 32;

  const float screen_ratio = float(SCREEN_WIDTH) / float(SCREEN_HEIGHT);
  const float ray_ratio = float(ray_width) / float(ray_height);

  // Clear
  glClearColor(g_clearColour.x, g_clearColour.y, g_clearColour.z,
               g_clearColour.w);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Setup render state: alpha-blending enabled, no face culling, no depth
  // testing, scissor enabled
  glEnable(GL_BLEND);
  glBlendEquation(GL_FUNC_ADD);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glDisable(GL_CULL_FACE);
  glDisable(GL_DEPTH_TEST);
  glEnable(GL_SCISSOR_TEST);

  if (g_rayresult.IsBindable()) {

    const float ortho_projection[4][4] = {
		{ ray_ratio / screen_ratio, 0, 0, 0},
		{0.0f, screen_ratio / ray_ratio, 0, 0},
		{0, 0, -1.0f, 0}, 
		{0, 0, 0, 1.0f}};

    ProgramData *prog = ProgramData::GetProg(PD_BASIC_TEX);
    prog->bind();
    prog->uniformList[0].SetMatrix4(&(ortho_projection[0][0]));

    glActiveTexture(GL_TEXTURE0 + prog->uniformList[1].loc);
    g_rayresult.BindTex();

    glBindVertexArray(g_testTexVertexArrays);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  } else {
    ProgramData *prog = ProgramData::GetProg(PD_BASIC_TEX);
    prog->bind();
    prog->uniformList[0].SetMatrix4(&g_drawtest[0][0]);

    // Draw Tex
    glActiveTexture(GL_TEXTURE0 + prog->uniformList[1].loc);
    g_imgTest.BindTex();

    glBindVertexArray(g_testTexVertexArrays);
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);

    glDrawArrays(GL_TRIANGLES, 0, 6);
  }

  // Compute Test 
  {
	  ProgramData *prog = ProgramData::GetProg(PD_COMPUTE_TEST);
	  prog->bind();


	  for (int i = 0; i < 1024; ++i) {
		  prog->uniformList[1].SetF((float)i*0.01f);
		  glDispatchCompute(512 / 16, 512 / 16, 1); // 512^2 threads in blocks of 16^2
		  glCheckError("Dispatch compute shader");
	  }
  }

  // Clean Up
  glBindVertexArray(NULL);
  glUseProgram(NULL);

  //
  ImGui::Render();
  SDL_GL_SwapWindow(window);

  glCheckError("Render");

  // Post Render Bullshit
  if (rs.want_draw) {

	  switch (rs.drawing)
	  {
	  case 	DRAW_FULL_FRAME:
		  Raycast(g_rayresult, rs);
		  g_rayresult.CopyToGPU();
		  rs.want_draw = false;
		  break;

	  case DRAW_ONE_LINE:
		  if ((rs.curr_pass < 0) || (rs.curr_pass >= rs.num_passes))
			  rs.curr_pass = 0;
		  break;

	  case DRAW_RANDOM:
		  if ((rs.curr_pass < 0) || (rs.curr_pass >= rs.num_passes))
			  rs.curr_pass = 0;
		  break;

	  default:
		  rs.want_draw = false;
		  break;
	  }

	  if (rs.want_draw) {
		  if ((false == g_rayresult.loaded) || (g_rayresult.width != ray_width) ||
			  (g_rayresult.height != ray_height))
			  g_rayresult.BlankImage(ray_width, ray_height);

		  Raycast(g_rayresult, rs);
		  g_rayresult.CopyToGPU();
		  ++rs.curr_pass;
	  }
  }

  if (g_request_save_file) {
	  g_request_save_file = false;

	  char filename[255] = { 0 };
	  
	  snprintf(filename, 255, "ray_%llu.png", g_walltime);
	  g_rayresult.SaveFile(filename);
  }
}
