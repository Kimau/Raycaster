#include "core.h"
#include "program.h"

#include "glprog.h"
#include "gltex.h"
#include "math/Cvector2.h"

#include <stdio.h>

GLuint g_testVertexArrays;
GLuint g_testTexVertexArrays;
ImageData g_imgTest = ImageData("test");
ImageData g_rayresult;

/////////////////////

bool loadBasicColorProgram() {
  ProgramData *prog = ProgramData::GetProg(PD_BASIC_RED);

  const GLchar *vertexSource = "#version 330\n"
                               "in vec4 Position;\n"
                               "in vec4 Color;\n"
                               "out vec4 Frag_Color;\n"
                               "uniform mat4 ProjMtx;\n"
                               "void main()\n"
                               "{\n"
                               "	Frag_Color = Color;\n"
                               "	gl_Position = ProjMtx * Position;\n"
                               "}\n";

  const GLchar *fragmentSource = "#version 330\n"
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

  const GLchar *vertexSource = "#version 330\n"
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

  const GLchar *fragmentSource =
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
    printf("\n\nBasic Colour Prog\nError %08X\n", glGetError());
    return false;
  }

  if (loadBasicTexProgram() == false) {
    printf("\n\nBasic Tex Prog\nError %08X\n", glGetError());
    return false;
  }

  printf("GL CHECK Error %08X\n", glGetError());

  g_imgTest.LoadFromFile("./img/test.png");
  if (g_imgTest.CopyToGPU() == false) {
    return false;
  }

  // Check for GL Errors
  int x = glGetError();
  if (x != GL_NO_ERROR) {
    printf("\nError %08X\n", x);
    return false;
  }

  return true;
}

bool setupOrthViewport(int width, int height) { return false; }

// Render
void render(SDL_Window *window) {
  int width = SCREEN_WIDTH;
  int height = SCREEN_HEIGHT;
  float invWidth = 1.0f / float(width);
  float invHeight = 1.0f / float(height);

  // Setup Render State
  glViewport(0, 0, width, height);

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
        {1.0f, 0, 0, 0}, {0.0f, 1.0f, 0, 0}, {0, 0, -1.0f, 0}, {0, 0, 0, 1.0f}};

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

  // Clean Up
  glBindVertexArray(NULL);
  glUseProgram(NULL);

  //
  ImGui::Render();
  SDL_GL_SwapWindow(window);

  // Post Render Bullshit
  if (g_request_brute_ray) {
    if ((false == g_rayresult.loaded) || (g_rayresult.width != width) ||
        (g_rayresult.height != height))
      g_rayresult.BlankImage(width, height);

    Raycast(g_rayresult);

    g_rayresult.CopyToGPU();
    g_request_brute_ray = false;
  }

  if (g_request_save_file) {
	  g_request_save_file = false;

	  char filename[255] = { 0 };
	  
	  sprintf(filename, "ray_%llu.png", g_walltime);
	  g_rayresult.SaveFile(filename);
  }
}
