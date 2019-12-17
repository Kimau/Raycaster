#include "core.h"
#include "gltex.h"
#include "math/CRay.h"
#include "program.h"

void Raycast(ImageData &output) {

  float invWidth = 1.0f / float(output.width);
  float invHeight = 1.0f / float(output.height);
  float ratio = float(output.width) / float(output.height);

  Ray cam;
  Vec3 topleft(-ratio, -1.0f, 0.0f);
  Vec3 hor(ratio * 2.0f, 0.0f, 0.0f);
  Vec3 ver(0.0f, 2.0f, 0.0f);

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = y * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
      float u = x * invWidth;
	  Ray ray(cam.origin(), cam.dir() + topleft + hor*u + ver*v);

      output.imgData[c++] = char(255 * u);
      output.imgData[c++] = char(255 * v);
      output.imgData[c++] = char(255 * 0.2f);
    }
  }
}