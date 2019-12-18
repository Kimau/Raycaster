#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include "program.h"

bool hit_sphere(const vec4& sphere, const ray& r) {
	vec3 oc = r.origin() - sphere.xyz();
	float a = r.dir().Dot(r.dir());
	float b = 2.0f * oc.Dot(r.dir());
	float c = oc.Dot(oc) - sphere.w*sphere.w;
	float discriminant = b * b - 4 * a*c;
	return (discriminant > 0);
}

vec3 color(const ray& r) {
	vec3 unit_direction = r.dir().getNormalized();
	float t = 0.5f*unit_direction.y + 1.0f;

	const vec3 ground(1.0f, 1.0f, 1.0f);
	const vec3 sky(0.5f, 0.7f, 1.0f);
	
	return lerp(sky, ground, t);
}

void Raycast(ImageData &output) {

  float invWidth = 1.0f / float(output.width);
  float invHeight = 1.0f / float(output.height);
  float ratio = float(output.width) / float(output.height);

  ray cam(vec3(), vec3(0.0f, 0.0f, -1.0f));
  vec3 topleft(-ratio, -1.0f, 0.0f);
  vec3 hor(ratio * 2.0f, 0.0f, 0.0f);
  vec3 ver(0.0f, 2.0f, 0.0f);

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = y * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
      float u = x * invWidth;
	  ray ray(cam.origin(), cam.dir() + topleft + hor*u + ver*v);

	  vec3 col = color(ray);

      output.imgData[c++] = char(255 * col.x);
      output.imgData[c++] = char(255 * col.y);
      output.imgData[c++] = char(255 * col.z);
    }
  }
}