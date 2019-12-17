#include "core.h"
#include "gltex.h"
#include "math/cray.h"
#include "program.h"

void Raycast(ImageData &output) {

  float invWidth = 1.0f / float(output.width);
  float invHeight = 1.0f / float(output.height);
  float ratio = float(output.width) / float(output.height);

  CRay cam;
  CVec3 topleft(-ratio, -1);
  CVec3 hor(ratio * 2.0f, 0, 0);
  CVec3 ver(0, 2.0f, 0);

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = y * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
      float u = x * invWidth;

      output.imgData[c++] = unsigned char(255 * u);
      output.imgData[c++] = unsigned char(255 * v);
      output.imgData[c++] = unsigned char(255 * 0.2f);
    }
  }
}