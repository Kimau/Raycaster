#pragma once

#include <GL/gl3w.h>
#include <vector>

#define PD_BASIC_TEX (0)
#define PD_BASIC_RED (1)

struct ShaderUniform {
  GLint loc;
  const GLchar *name;

  ShaderUniform(const GLchar *_name);
  bool PostLink(GLuint progID);

  void SetF(float x);
  void SetF(float x, float y);
  void SetF(float x, float y, float z);
  void SetF(float x, float y, float z, float w);
  void Set(int x);
  void Set(int x, int y);
  void Set(int x, int y, int z);
  void Set(int x, int y, int z, int w);
  void SetMatrix2(const float *val, int count = 1, bool transpose = false);
  void SetMatrix3(const float *val, int count = 1, bool transpose = false);
  void SetMatrix4(const float *val, int count = 1, bool transpose = false);
};

struct ShaderAttrib {
  GLint loc;
  const GLchar *name;

  ShaderAttrib(const GLchar *_name);
  bool PostLink(GLuint progID);

  void EnableArray();
  void DisableArray();
};

struct ProgramData {
  bool isValid;
  GLuint progID;
  std::vector<GLuint> shaderList;
  std::vector<ShaderUniform> uniformList;
  std::vector<ShaderAttrib> attribList;

  void create();
  void destroy();

  bool compileAttachShader(const GLchar *source, GLenum type);
  bool attachShader(GLuint shader);
  void setupAttrib(const GLchar *name);
  void setupUniform(const GLchar *name);

  bool link();
  bool bind();

  static ProgramData *GetProg(int ID);
};

void unbindProgram();

GLuint compileShader(const GLchar *source, GLenum type);

void printProgramLog(GLuint program);
void printShaderLog(GLuint shader);

// Buffer Stuff
void genBasicFloatNBuffer(GLuint index, ShaderAttrib &attrib, GLfloat *vertData,
                          GLuint length, GLuint numFloatrs = 4);