#pragma once

#include <GL/gl3w.h>

struct ImageData {
  bool loaded = false;
  int width = -1;
  int height = -1;
  int imgType = -1;
  GLuint glTex = -1;
  const char *name = nullptr;
  char *imgData = nullptr;

  ImageData() {}
  ImageData(const char *friendlyName);

  void LoadFromFile(const char *filename);
  void BlankImage(int width, int height);
  bool CopyToGPU();
  void FreeImage();

  void BindTex();

  inline bool IsBindable() const { return (glTex != -1); }
};
