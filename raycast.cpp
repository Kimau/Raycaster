#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include "program.h"
#include <stdlib.h>

float hit_sphere(const vec4& sphere, const ray& r) {
	vec3 oc = r.origin() - sphere.xyz();
	float a = r.dir().Dot(r.dir());
	float b = 2.0f * oc.Dot(r.dir());
	float c = oc.Dot(oc) - sphere.w*sphere.w;
	float discriminant = b * b - 4 * a*c;

	if (discriminant < 0)
		return -1.0f;

	return (-b - sqrtf(discriminant)) / (2.0f*a);
}


void CastBall(const vec4& ball, const ray &r, vec4 &col)
{
	float t = hit_sphere(ball, r);
	if ((t > 0.0) && ((col.w < 0) || (col.w > t))) {
		vec3 n = r.PointAt(t) - vec3(0.0f, 0.0f, -1.0f);
		col = vec4(vec3(n)*0.5f + vec3(1.0f, 1.0f, 1.0f), t);
	}
}


void CastFloor(vec4 floor, const ray &r, vec4 &col)
{
	float t = intersectPlane(floor, r);
	if ((t > 0.0) && ((col.w < 0) || (col.w > t))) {
		vec3 fpt = r.a + r.b.getNormalized()*t;

		if ((sinf(fpt.x*PI*1.0f) > 0) != (sinf(fpt.z*PI*1.0f) > 0))
			col = vec4(0.6f, 0.6f, 0.6f, t);
		else
			col = vec4(0.8f, 0.8f, 0.8f, t);
	}
}

vec3 color(const ray& r) {
	vec3 unit_direction = r.dir().getNormalized();

	const vec3 ground(1.0f, 1.0f, 1.0f);
	const vec3 sky(0.5f, 0.7f, 1.0f);
	vec4 col = vec4(lerp(ground, sky, -unit_direction.y), -1.0f);

	CastBall(vec4(-3.0f, 0.0f, -4.0f, 0.5f), r, col);
	CastBall(vec4(+3.0f, 0.0f, -4.0f, 0.5f), r, col);
	CastBall(vec4(0.0f, 1.3f, -4.0f, 0.5f), r, col);

	// Hit Ground
	CastFloor(vec4(0.0f, 1.0f, 0.0f, 0.0f), r, col);

	// Hit Sky
	return col.xyz();
}

void Raycast(ImageData &output) {

  float invWidth = 1.0f / float(output.width);
  float invHeight = 1.0f / float(output.height);
  float ratio = float(output.width) / float(output.height);

  ray cam(g_campos, g_camdir.getNormalized());
  vec3 topleft(-ratio, -1.0f, -1.0f);
  vec3 hor(ratio * 2.0f, 0.0f, 0.0f);
  vec3 ver(0.0f, 2.0f, 0.0f);

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = y * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
		if ((rand() % 10) != 0) {
			c += 3;
			continue;
		}

      float u = x * invWidth;
	  ray ray(cam.origin(), cam.dir() + topleft + hor*u + ver*v);

	  vec3 col = color(ray);

      output.imgData[c++] = char(255 * col.x);
      output.imgData[c++] = char(255 * col.y);
      output.imgData[c++] = char(255 * col.z);
    }
  }
}