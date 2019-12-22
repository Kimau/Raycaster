#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include "program.h"
#include <stdlib.h>
#include <vector>

// Z Up

struct hit_record
{
	float t = 0.0f;
	vec3 p;
	vec3 n;
	u32 hitid = 0;
};

enum solid_type
{
	SOLID_SKY,
	SOLID_BALL,
	SOLID_PLANE,
	SOLID_DISC,
	NOOF_SOLID_TYPES
};

struct solidhit {
	vec4 v;
	solid_type kind = SOLID_SKY;	
	size_t sz_extra = 0;
	void* extra = nullptr;


	// Creation
	solidhit(solid_type _t, vec4 _v) : v(_v), kind(_t), sz_extra(0), extra(nullptr) {}
	solidhit(const solidhit& o) : v(o.v), kind(o.kind) {
		set_extra(o.extra, o.sz_extra);
	}

	~solidhit() {
		free_data();
	}

	void set_extra(void* ex, size_t sz) {
		free_data();
		sz_extra = sz;
		extra = malloc(sz_extra);
		memcpy(extra, ex, sz_extra);
	}

	void free_data() {
		if (extra) free(extra);
		sz_extra = 0;
	}

	static solidhit create_ball(vec4 v) { return solidhit(SOLID_BALL, v); }
	static solidhit create_plane(vec4 v) { return solidhit(SOLID_PLANE, v); }
	static solidhit create_disc(vec4 v, float r);
	static solidhit create_skysphere(vec4 v, vec4* grad, u32 num_grad) {
		solidhit sh(SOLID_SKY, v);
		sh.set_extra(grad, sizeof(vec4)*num_grad);
		return sh;
	}
};


struct extra_disc {
	float rad = 1.0f;
};

solidhit solidhit::create_disc(vec4 v, float r) {
	solidhit sh(SOLID_DISC, v);
	extra_disc ex{ r };
	sh.set_extra(&ex, sizeof(ex));
	return sh;
}


std::vector<solidhit> solids;

void init_world() {
	solids.push_back(solidhit::create_ball(vec4(-3.0f, 10.0f, 2.0f, 0.5f)));
	solids.push_back(solidhit::create_ball(vec4( 0.0f, 10.0f, 1.0f, 0.5f)));
	solids.push_back(solidhit::create_ball(vec4(+3.0f, 10.0f, 0.0f, 0.5f)));
	solids.push_back(solidhit::create_ball(vec4(+4.0f, 10.0f, 0.0f, 0.9f)));
	solids.push_back(solidhit::create_plane(vec4(0.0f, 0.0f, 1.0f, 0.0f)));

	vec4 c[] = {
		vec4(1.0f, 1.0f, 1.0f,0.0f),
		vec4(0.5f, 0.7f, 1.0f,0.8f)
	};
	solids.push_back(solidhit::create_skysphere(vec4(0.0f, 0.0f, 0.0f, 1000.0f), c, 2));
}

bool hit_sphere(const ray3& r, const solidhit& tar, hit_record* hit_data) {
	vec3 oc = r.origin() - tar.v.xyz();
	const float oc_mag_sq = oc.SqrdMag();
	if (oc_mag_sq < (tar.v.w*tar.v.w)) {
		if (hit_data != nullptr) {
			hit_data->t = tar.v.w - sqrtf(oc_mag_sq);
			hit_data->p = r.PointAt(hit_data->t);
			hit_data->n = (r.PointAt(hit_data->t) - tar.v.xyz()) / tar.v.w;
		}			
		return true;
	}

	float a = r.dir().Dot(r.dir());
	float b = 2.0f * oc.Dot(r.dir());
	float c = oc.Dot(oc) - tar.v.w*tar.v.w;
	float discriminant = b * b - 4 * a*c;

	if (discriminant < 0)
		return false;

	if (hit_data != nullptr) {
		hit_data->t = (-b - sqrtf(discriminant)) / (2.0f*a);
		hit_data->p = r.PointAt(hit_data->t);
		hit_data->n = (r.PointAt(hit_data->t) - tar.v.xyz()) / tar.v.w;
	}

	return true;
}

bool hit_plane(const ray3& r, const solidhit& tar, hit_record* hit_data) {
	const vec3 pnorm = tar.v.xyz();
	float denom = pnorm.Dot(r.dir());

	if (fabsf(denom) <= 0.000001f) return false;

	const vec3 plane0 = tar.v.xyz() * tar.v.w;
	const vec3 p0l0 = plane0 - r.origin();
	const float t = p0l0.Dot(pnorm) / denom;

	if (t < 0.0f) return false;

	if (hit_data != nullptr) {
		hit_data->t = t;
		hit_data->p = r.PointAt(hit_data->t);
		hit_data->n = pnorm;
	}

	return true;
}


bool hit_disc(const ray3& r, const solidhit& tar, hit_record* hit_data) {
	const vec3 pnorm = tar.v.xyz();
	float denom = pnorm.Dot(r.dir());

	if (fabsf(denom) <= 0.000001f) return false;

	const vec3 plane0 = tar.v.xyz() * tar.v.w;
	const vec3 p0l0 = plane0 - r.origin();
	const float t = p0l0.Dot(pnorm) / denom;
	const vec3 p = r.PointAt(t);

	const float dist2 = (p - plane0).SqrdMag();

	const extra_disc*  disc = (extra_disc*)tar.extra;

	if (dist2 > disc->rad*disc->rad) return false;

	if (hit_data != nullptr) {

		hit_data->t = t;
		hit_data->p = p;
		hit_data->n = pnorm;
	}

	return true;
}

bool hit(const ray3& r, const solidhit& tar, hit_record* hit_data) {
	switch (tar.kind) {
	case SOLID_SKY:   return hit_sphere(r, tar, hit_data);
	case SOLID_BALL:  return hit_sphere(r, tar, hit_data);
	case SOLID_PLANE: return hit_plane(r, tar, hit_data);
	case SOLID_DISC:  return hit_disc(r, tar, hit_data);
	}

	return false;
}

vec3 color(const ray3& r) {
	hit_record best_hit = {
		FLT_MAX,
		vec3(0.0f, 0.0f, 0.0f),
		vec3(0.0f, 0.0f, 0.0f),
		u32(solids.size()+1)
	};

	hit_record test_hit;

	for (int i = 0; i < solids.size(); ++i) {
		const solidhit& sh = solids[i];

		if (hit(r, sh, &test_hit) == false) continue;
		
		if (test_hit.t > best_hit.t) continue;
		
		best_hit = test_hit;
		best_hit.hitid = i;
	}

	// Determine Colour
	{
		if(best_hit.hitid > solids.size())
			return vec3(0.0f, 0.0f, 0.0f);

		const solidhit& sh = solids[best_hit.hitid];
		switch (sh.kind) {
		case SOLID_BALL: return (best_hit.n * 0.5f + vec3(0.5f, 0.5f, 0.5f));
		case SOLID_PLANE:
			if ((sinf(best_hit.p.x*PI*1.0f) > 0) != (sinf(best_hit.p.y*PI*1.0f) > 0))
				return vec3(0.6f, 0.6f, 0.6f);
			else
				return vec3(0.8f, 0.8f, 0.8f);
		case SOLID_SKY: return vec3(0.0f, 
			saturate(0.5f + best_hit.n.z *0.5f), 
			saturate(0.5f - best_hit.n.z *0.5f));

		case SOLID_DISC: return vec3(1.0f, 0.0f, 0.0f);
		}

		return vec3(0.0f, 0.0f, 0.0f);
	}
}

inline float random_float() {
	return float(rand()) / float(RAND_MAX + 1.0f);
}

void Raycast(ImageData &output, const raycast_state& s) {

  const float invWidth = 1.0f / float(output.width);
  const float invHeight = 1.0f / float(output.height);
  const float ratio = float(output.width) / float(output.height);
  const int limit = output.width * output.height * 3;

  const float inv_ns = 1.0f / float(s.num_samples);
  const vec3 perp = s.up * s.cam.dir();

  // Fake Animating world Bullshit which should be in update
  if (solids.empty()) {
	  init_world();
  }
  solids[0].v.w = 0.25f + (sinf(g_walltime * 0.001f) + 1.0f) * 1.0f;
  solids[2].v.z = sinf(g_walltime * 0.001f) * 2.0f;
  
  
  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = (output.height - y) * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
		if ((s.pass_break > 1) && ((rand() % s.pass_break) != 0)) {
			c += 3;
			continue;
		}

      float u = x * invWidth;
	  vec3 col(0.0f, 0.0f, 0.0f);
	  ray3 r(s.cam.origin(), s.cam.dir() + s.up * (v - 0.5f) + perp * (u - 0.5f) * ratio);
	  r.b.Normalize();

	  if (s.num_samples > 1) {
		  for (int n = 0; n < s.num_samples; ++n) {
			  col += color(ray3(r.a, r.b
				  + s.up * ((random_float()*0.8f - 0.4f)*invHeight)
				  + perp * ((random_float()*0.8f - 0.4f)*invWidth)));
		  }

		  col *= inv_ns;
	  }
	  else {
		  col = color(r);
	  }

      output.imgData[c++] = char(255 * col.x);
      output.imgData[c++] = char(255 * col.y);
      output.imgData[c++] = char(255 * col.z);
    }
  }

  assert(c <= limit);

  if (s.border) {
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