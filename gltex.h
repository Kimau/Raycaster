#pragma once

#include "types.h"
#include <GL/gl3w.h>

struct ImageData {
	const char *name = nullptr;
	u8 *imgData = nullptr;

  int imgType = -1;
  int width = -1;
  int height = -1;
  GLuint glTex = -1;

  bool loaded = false;

  ImageData() {}
  ImageData(const char *friendlyName);

  void LoadFromFile(const char *filename);
  int SaveFile(const char *filename);
  void BlankImage(int width, int height);
  bool CopyToGPU();
  void FreeImage();

  void BindTex();

  inline bool IsBindable() const { return (glTex != -1); }
};
