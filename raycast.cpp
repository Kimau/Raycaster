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
		vec3 n = r.PointAt(t) - ball.xyz();
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

void Raycast(ImageData &output, int pass_break /*= 1*/, bool border /*= true*/) {

  const float invWidth = 1.0f / float(output.width);
  const float invHeight = 1.0f / float(output.height);
  const float ratio = float(output.width) / float(output.height);
  const int limit = output.width * output.height * 3;

  const ray cam_dir(g_campos, g_camdir.getNormalized());
  const vec3 top_left(-ratio, -1.0f, -1.0f);
  const vec3 hor(ratio * 2.0f, 0.0f, 0.0f);
  const vec3 ver(0.0f, 2.0f, 0.0f);

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = y * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
		if ((pass_break > 1) && ((rand() % pass_break) != 0)) {
			c += 3;
			continue;
		}

      float u = x * invWidth;
	  ray ray(cam_dir.origin(), cam_dir.dir() + top_left + hor*u + ver*v);

	  vec3 col = color(ray);

      output.imgData[c++] = char(255 * col.x);
      output.imgData[c++] = char(255 * col.y);
      output.imgData[c++] = char(255 * col.z);
    }
  }

  assert(c <= limit);

  if (border) {
	  c = (0 * output.width * 3);
	  for (int x = output.width - 1; x >= 0; --x) {
		  output.imgData[c++] = char(0);
		  output.imgData[c++] = char(0);
		  output.imgData[c++] = char(0);
	  }
	  c = ((output.height - 1) * output.width * 3);
	  for (int x = output.width - 1; x >= 0; --x) {
		  output.imgData[c++] = char(0);
		  output.imgData[c++] = char(0);
		  output.imgData[c++] = char(0);
	  }

	  for (int y = output.height - 1; y >= 0; --y) {
		  c = (y * output.width) * 3;
		  output.imgData[c + 0] = char(0);
		  output.imgData[c + 1] = char(0);
		  output.imgData[c + 2] = char(0);

		  c = ((y + 1) * output.width - 1) * 3;
		  output.imgData[c + 0] = char(0);
		  output.imgData[c + 1] = char(0);
		  output.imgData[c + 2] = char(0);
	  }

	  assert(c <= limit);
  }
}