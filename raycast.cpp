#include "core.h"
#include "gltex.h"
#include "math/CRay.h"
#include "program.h"

Vec3 color(const Ray& r) {
	Vec3 unit_direction = r.dir().getNormalized();
	float t = 0.5f*(unit_direction.y + 1.0f);

	const Vec3 ground(1.0f, 1.0f, 1.0f);
	const Vec3 sky(0.5f, 0.7f, 1.0f);
	
	return lerp(sky, ground, t);
}

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

	  Vec3 col = color(ray);

      output.imgData[c++] = char(255 * col.x);
      output.imgData[c++] = char(255 * col.y);
      output.imgData[c++] = char(255 * col.z);
    }
  }
}