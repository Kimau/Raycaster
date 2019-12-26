#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include "program.h"
#include <stdlib.h>
#include <vector>

// Z Up


inline float random_float() { return float(rand()) / float(RAND_MAX + 1.0f); }
inline vec3 random_in_unit_sphere() { return vec3(random_float()*1.99f - 1.0f, random_float()*1.99f - 1.0f, random_float()*1.99f - 1.0f); }

const u32 num_pastels = 33;
const vec3 pastel[] = {
	vec3(0.4666666666666667f,0.8666666666666667f,0.4666666666666667f),
	vec3(0.5372549019607843f,0.8117647058823529f,0.9411764705882353f),
	vec3(0.6f,0.7725490196078432f,0.7686274509803922f),
	vec3(0.6039215686274509f,0.8705882352941177f,0.8588235294117647f),
	vec3(0.6666666666666666f,0.5803921568627451f,0.6f),
	vec3(0.6666666666666666f,0.9411764705882353f,0.8196078431372549f),
	vec3(0.6980392156862745f,0.984313725490196f,0.6470588235294118f),
	vec3(0.7019607843137254f,0.6196078431372549f,0.7098039215686275f),
	vec3(0.7411764705882353f,0.6901960784313725f,0.8156862745098039f),
	vec3(0.7450980392156863f,0.9058823529411765f,0.6470588235294118f),
	vec3(0.7450980392156863f,0.9921568627450981f,0.45098039215686275f),
	vec3(0.7568627450980392f,0.7764705882352941f,0.9882352941176471f),
	vec3(0.5137254901960784f,0.4117647058823529f,0.3254901960784314f),
	vec3(0.7764705882352941f,0.6431372549019608f,0.6431372549019608f),
	vec3(0.7843137254901961f,1.0f,0.6901960784313725f),
	vec3(0.796078431372549f,0.6f,0.788235294117647f),
	vec3(0.807843137254902f,0.9411764705882353f,0.8f),
	vec3(0.8117647058823529f,0.8117647058823529f,0.7686274509803922f),
	vec3(0.8392156862745098f,1.0f,0.996078431372549f),
	vec3(0.8470588235294118f,0.6313725490196078f,0.7686274509803922f),
	vec3(0.8705882352941177f,0.6470588235294118f,0.6431372549019608f),
	vec3(0.8705882352941177f,0.9254901960784314f,0.8823529411764706f),
	vec3(0.8745098039215686f,0.8470588235294118f,0.8823529411764706f),
	vec3(0.8980392156862745f,0.8509803921568627f,0.8274509803921568f),
	vec3(0.9137254901960784f,0.8196078431372549f,0.7490196078431373f),
	vec3(0.9568627450980393f,0.6039215686274509f,0.7607843137254902f),
	vec3(0.9568627450980393f,0.7490196078431373f,1.0f),
	vec3(0.9921568627450981f,0.9921568627450981f,0.5882352941176471f),
	vec3(1.0f,0.4117647058823529f,0.3803921568627451f),
	vec3(1.0f,0.5882352941176471f,0.30980392156862746f),
	vec3(1.0f,0.596078431372549f,0.6f),
	vec3(1.0f,0.7176470588235294f,0.807843137254902f),
	vec3(0.792156862745098f,0.6078431372549019f,0.9686274509803922f)
};

// Hit System
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
	solidhit() {}
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


solidhit* list_solids = nullptr;
u64 num_solids = 0;

void init_world() {
	if (list_solids != nullptr) delete list_solids;
	list_solids = new solidhit[6];
	num_solids = 0;

	// Sky box must ALWAYS be first
	vec4 c[] = {
		vec4(1.0f, 1.0f, 1.0f,0.0f),
		vec4(0.5f, 0.7f, 1.0f,0.8f)
	};
	list_solids[num_solids++] = solidhit::create_skysphere(vec4(0.0f, 0.0f, 0.0f, 1000.0f), c, 2);

	list_solids[num_solids++] = solidhit::create_ball(vec4(-3.0f, 10.0f, 2.0f, 0.5f));
	list_solids[num_solids++] = solidhit::create_ball(vec4( 0.0f, 10.0f, 1.0f, 0.5f));
	list_solids[num_solids++] = solidhit::create_ball(vec4(+2.7f, 10.5f, 0.7f, 0.5f));
	list_solids[num_solids++] = solidhit::create_ball(vec4(+2.7f, 10.5f, 1.7f, 0.5f));
	list_solids[num_solids++] = solidhit::create_ball(vec4(+4.0f, 10.0f, 1.0f, 0.9f));
	list_solids[num_solids++] = solidhit::create_plane(vec4(0.0f, 0.0f, 1.0f, 0.0f));
}

bool hit_sphere(const ray3& r, const solidhit& tar, float tmin = 0.0f, float tmax = FLT_MAX, hit_record* hit_data = nullptr) {
	const vec3 tp = tar.v.xyz();
	const vec3 oc = r.origin() - tp;
	const float oc_mag_sq = oc.SqrdMag();
	if (oc_mag_sq < (tar.v.w*tar.v.w)) {
		float t = tar.v.w - sqrtf(oc_mag_sq);
		if ((t < tmin) || (t > tmax)) return false;

		if (hit_data != nullptr) {
			hit_data->t = tar.v.w - sqrtf(oc_mag_sq);
			hit_data->p = r.PointAt(hit_data->t);
			hit_data->n = (r.PointAt(hit_data->t) - tp) / tar.v.w;
		}
		return true;
	}

	float a = r.dir().Dot(r.dir());
	float b = 2.0f * oc.Dot(r.dir());
	float c = oc.Dot(oc) - tar.v.w*tar.v.w;
	float discriminant = b * b - 4.0f*a*c;

	if (discriminant < 0)
		return false;

	float t = (-b - sqrtf(discriminant)) / (2.0f*a);
	if ((t < tmin) || (t > tmax)) return false;

	if (hit_data != nullptr) {		
		hit_data->t = t;
		hit_data->p = r.PointAt(hit_data->t);
		hit_data->n = (r.PointAt(hit_data->t) - tp) / tar.v.w;
	}

	return true;
}

bool hit_sky_sphere(const ray3& r, const solidhit& tar, float tmin = 0.0f, float tmax = FLT_MAX, hit_record* hit_data = nullptr) {
	if (hit_data != nullptr) {
		const vec3 tp = tar.v.xyz();
		const vec3 oc = r.origin() - tp;
		float oc_mag_sq = oc.SqrdMag();

		float rad = tmax + sqrtf(oc_mag_sq);
		hit_data->p = r.PointAt(tmax);
		hit_data->n = (r.PointAt(tmax) - tp) / rad;
		hit_data->t = tmax;
	}

	return true;
}

bool hit_plane(const ray3& r, const solidhit& tar, float tmin = 0.0f, float tmax = FLT_MAX, hit_record* hit_data = nullptr) {
	const vec3 pnorm = tar.v.xyz();
	float denom = pnorm.Dot(r.dir());

	if (fabsf(denom) <= 0.000001f) return false;

	const vec3 plane0 = pnorm * tar.v.w;
	const vec3 p0l0 = plane0 - r.origin();
	const float t = p0l0.Dot(pnorm) / denom;

	if ((t < tmin) || (t > tmax)) return false;

	if (hit_data != nullptr) {
		hit_data->t = t;
		hit_data->p = r.PointAt(hit_data->t);
		hit_data->n = pnorm;
	}

	return true;
}


bool hit_disc(const ray3& r, const solidhit& tar, float tmin = 0.0f, float tmax = FLT_MAX, hit_record* hit_data = nullptr) {
	const vec3 pnorm = tar.v.xyz();
	float denom = pnorm.Dot(r.dir());

	if (fabsf(denom) <= 0.000001f) return false;

	const vec3 plane0 = tar.v.xyz() * tar.v.w;
	const vec3 p0l0 = plane0 - r.origin();
	const float t = p0l0.Dot(pnorm) / denom;

	if ((t < tmin) || (t > tmax)) return false;

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

bool hit(const ray3& r, const solidhit& tar, float tmin =0.0f, float tmax=FLT_MAX, hit_record* hit_data = nullptr) {
	switch (tar.kind) {
	case SOLID_SKY:   return hit_sky_sphere(r, tar, tmin, tmax, hit_data);
	case SOLID_BALL:  return hit_sphere(r, tar, tmin, tmax, hit_data);
	case SOLID_PLANE: return hit_plane(r, tar, tmin, tmax, hit_data);
	case SOLID_DISC:  return hit_disc(r, tar, tmin, tmax, hit_data);
	}

	return false;
}

hit_record best_hit;
hit_record test_hit;

vec3 color(const ray3& r, float tmin, float tmax) {
	tmax *= 0.9f; // Degrade Fast
	best_hit.t = tmax;	

	for (int i = 0; i < num_solids; ++i) {
		if (false == hit(r, list_solids[i], tmin, best_hit.t, &test_hit))
			continue;

		if (test_hit.t > best_hit.t) continue;

		best_hit = test_hit;
		best_hit.hitid = i;
	}

	// Determine Colour
	if (best_hit.hitid < num_solids) {
		const solidhit& sh = list_solids[best_hit.hitid];

		tmax -= (best_hit.t > tmin)?best_hit.t:tmin;
		if ((sh.kind != SOLID_SKY) && (tmax < 0.0f)) 
			return vec3(0.0f, 0.0f, 0.0f);

		switch (sh.kind) {
		case SOLID_BALL: {
			const vec3& c = pastel[best_hit.hitid % num_pastels];

			// Pure reflection
			if (fabsf(best_hit.n.SqrdMag() - 1.0f) > 0.0001f)
				best_hit.n.Normalize();
			vec3 bounce = color(ray3(best_hit.p, best_hit.n + poissonSphere8(rand()%8) * 0.1f), tmin, tmax);
			return bounce;
			float b = saturate(c.Dot(bounce));
			return lerp(bounce*b, c, b); 

			// return (best_hit.n * 0.5f + vec3(0.5f, 0.5f, 0.5f)); Normal
		}
		case SOLID_PLANE: {
			float shade = 0.8f;
			if ((sinf(best_hit.p.x*PI*1.0f) > 0) != (sinf(best_hit.p.y*PI*1.0f) > 0))
				shade = 0.6f;

			if (fabsf(best_hit.n.SqrdMag() - 1.0f) > 0.0001f)
				best_hit.n.Normalize();
			return color(ray3(best_hit.p, best_hit.n + poissonSphere8(rand()%8) * 0.1f), tmin, tmax) * shade;
			// return vec3(shade, shade, shade);
		}


		case SOLID_SKY: {
			const vec4 a(1.0f, 1.0f, 1.0f, 0.0f);
			const vec4 b(0.5f, 0.7f, 1.0f, 0.8f);
			return lerp(a.xyz(), b.xyz(), saturate((best_hit.n.z - a.w) / (b.w - a.w)));

			// Proper method
			vec4* skyGrad = (vec4*)sh.extra;
			int imax = int(sh.sz_extra / sizeof(vec4));

			int i = 0;
			while ((i < imax) && (skyGrad[i].w < best_hit.n.z)) ++i;

			if (i < 2) return skyGrad[0].xyz();
			if (i == imax) return skyGrad[imax - 1].xyz();
			return lerp(skyGrad[i - 1].xyz(), skyGrad[i].xyz(),
				(best_hit.n.z - skyGrad[i - 1].w) / (skyGrad[i].w - skyGrad[i - 1].w));


		}

		case SOLID_DISC: return vec3(1.0f, 0.0f, 0.0f);
		}
	}

	return vec3(0.0f, 0.0f, 0.0f);
}

void Raycast(ImageData &output, const raycast_state& s) {

  const float invWidth = 1.0f / float(output.width);
  const float invHeight = 1.0f / float(output.height);
  const float ratio = float(output.width) / float(output.height);
  const int limit = output.width * output.height * 3;

  const float inv_ns = 1.0f / float(s.num_samples);
  const vec3 perp = s.up * s.cam.dir();

  // Fake Animating world Bullshit which should be in update
  if (list_solids == nullptr) 
	  init_world();

  list_solids[1].v.w = 0.25f + (sinf(g_ticks_since_start * 0.001f) + 1.0f) * 1.0f;
  list_solids[2].v.z = sinf(g_ticks_since_start * 0.001f) * 2.0f;

  int pass_tick = (s.pass_break>0)?(rand() % s.pass_break):0;

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = (output.height - y) * invHeight;
    for (int x = output.width - 1; x >= 0; --x) {
		if (s.pass_break > 1) {
			if (--pass_tick > 0) {
				c += 3;
				continue;
			}
			else {
				pass_tick = rand() % s.pass_break;
			}
		}

      float u = x * invWidth;
	  vec3 col(0.0f, 0.0f, 0.0f);
	  ray3 r(s.cam.origin(), s.cam.dir() + s.up * (v - 0.5f) + perp * (u - 0.5f) * ratio);
	  r.b.Normalize();

	  if (s.num_samples > 1) {
		  for (int n = 0; n < s.num_samples; ++n) {
			  const vec2& poi = (s.num_samples >8)?poissonDisc50(n):poissonDisc8(n);
			  col += color(ray3(r.a, r.b
				  + perp * poi.x*invWidth
				  + s.up * poi.y*invHeight),
				  0.000001f, 300.0f);
		  }

		  col *= inv_ns;
	  }
	  else {
		  col = color(r, 0.00001f, 300.0f);
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