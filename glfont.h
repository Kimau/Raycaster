#pragma once

#include "gltex.h"
#include <vector>

class glFont {
public:
  glFont();
  ~glFont();
  void Init(const char *filename = "c:/windows/fonts/times.ttf");

  // GenBindVertList
  // Generates a Interleaved UV/XY float4 single array
  GLuint GenBindVertList(float x, float y, const std::vector<char> &text);

  ImageData bakedFont;
  float pixelHeight;
  int codePointStart;
  int codePointEnd;

private:
  void *cdata; // stbtt_bakedchar[]
};
