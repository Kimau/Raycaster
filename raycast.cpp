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
enum material_type {
	MATERIAL_FLAT_COLOUR,
	MATERIAL_DIFFUSE,	// rgba : [roughness,?,?,?]
	MATERIAL_GLASS,
	MATERIAL_SKY,
	MATERIAL_NORMALS,
	MATERIAL_TILES,
	NOOF_MATERIAL_TYPES
};

enum solid_type
{
	SOLID_SKY,
	SOLID_BALL,
	SOLID_PLANE,
	SOLID_DISC,
	NOOF_SOLID_TYPES
};

class world;
class hit_record;
class material;
class solidhit;

class hit_record
{
public:
	float t = 0.0f;
	vec3 p;
	vec3 n;
	u32 hitid = 0;
};

class material {
public:
	virtual material_type get_type() = 0;
	virtual bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) = 0;
};

class mat_flat :public  material {
public:
	vec3 rgb;
	mat_flat(vec3 colour) : rgb(colour) {}

	inline material_type get_type() { return MATERIAL_FLAT_COLOUR; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) override {
		out_colour = rgb;
		return true;
	}
};

class mat_normal :public  material {
public:
	mat_normal() {}

	inline material_type get_type() { return MATERIAL_NORMALS; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) override {
		out_colour = hit.n * 0.5f + vec3(0.5f, 0.5f, 0.5f);
		return true;
	}
};

class mat_diffuse : public material {
public:
	float r = 0.0f;
	float m = 1.0f;

	mat_diffuse(float roughness, float mirror) : r(roughness), m(mirror) {}

	inline material_type get_type() { return MATERIAL_DIFFUSE; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) override {
		ray.a = hit.p;
		ray.b.Reflect(hit.n);

		if (r > 0.0f)
			ray.b += poissonSphere8(rand() % 8) * saturate(r);			

		return false;
	}
};


class mat_glass : public material {
public:
	float r = 0.0f;
	float m = 1.0f;

	mat_glass(float roughness, float mirror) : r(roughness), m(mirror) {}

	inline material_type get_type() { return MATERIAL_GLASS; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) override {
		ray.a = hit.p;
		ray.b.Refract(hit.n);

		if (r > 0.0f)
			ray.b += poissonSphere8(rand() % 8) * saturate(r);

		return false;
	}
};

class mat_tiles : public material {
public:
	float r = 0.0f;
	float m = 1.0f;

	mat_tiles(float roughness, float mirror) : r(roughness), m(mirror) {}

	inline material_type get_type() { return MATERIAL_DIFFUSE; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) override {
		if ((sinf(hit.p.x) > 0) ^ (cosf(hit.p.y) > 0)) {
			ray.a = hit.p;
		}
		else {
			ray.a = hit.p;
			ray.b.Reflect(hit.n);
		}

		if (r > 0.0f)
			ray.b += poissonSphere8(rand() % 8) * saturate(r);

		return false;
	}
};

class mat_sky : public material {
public:
	vec4* cols = nullptr;
	u16 num_cols = 0;
	
	mat_sky(std::vector<vec4> colours) {
		num_cols = u16(colours.size());
		cols = new vec4[num_cols];
		for (u16 i = 0; i < num_cols; ++i) 
			cols[i] = colours[i];
	}
	~mat_sky() {
		delete cols;
	}

	inline material_type get_type() { return MATERIAL_SKY; };
	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) override {
		// Proper method


		int i = 0;
		while ((i < num_cols) && (cols[i].w < hit.n.z)) ++i;

		if (i == 0) out_colour = cols[0].xyz();
		else if (i >= num_cols) out_colour = cols[num_cols-1].xyz();
		else out_colour = lerp(cols[i - 1].xyz(), cols[i].xyz(), (hit.n.z - cols[i - 1].w) / (cols[i].w - cols[i - 1].w));

		return true;
	}
};

class solidhit {
public:

	virtual bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const = 0;
	virtual u16 get_material() = 0;
	virtual void set_material(u16 mat) = 0;
	virtual solid_type get_type() = 0;
};

class solid_ball : public solidhit {
public:
	vec4 v;
	u16 mat = 0;

	solid_ball(vec4 vec) : v(vec), mat(0) {}

	inline u16 get_material() { return mat;  }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_BALL; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const  {
		const vec3 tp = v.xyz();
		const vec3 oc = r.origin() - tp;
		const float oc_mag_sq = oc.SqrdMag();
		if (oc_mag_sq < (v.w*v.w)) {
			float t = v.w - sqrtf(oc_mag_sq);
			if ((t < tmin) || (t > tmax)) return false;

			if (hit_data != nullptr) {
				hit_data->t = v.w - sqrtf(oc_mag_sq);
				hit_data->p = r.PointAt(hit_data->t);
				hit_data->n = (r.PointAt(hit_data->t) - tp) / v.w;
			}
			return true;
		}

		float a = r.dir().Dot(r.dir());
		float b = 2.0f * oc.Dot(r.dir());
		float c = oc.Dot(oc) - v.w*v.w;
		float discriminant = b * b - 4.0f*a*c;

		if (discriminant < 0)
			return false;

		float t = (-b - sqrtf(discriminant)) / (2.0f*a);
		if ((t < tmin) || (t > tmax)) return false;

		if (hit_data != nullptr) {
			hit_data->t = t;
			hit_data->p = r.PointAt(hit_data->t);
			hit_data->n = (r.PointAt(hit_data->t) - tp) / v.w;
		}

		return true;
	}
};

class solid_skysphere : public solidhit {
public:
	float rad = 0.0f;
	u16 mat = 0;

	solid_skysphere(float radius) : rad(radius) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_SKY; }
	
	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override {
		if (hit_data != nullptr) {
			const vec3 oc = r.origin();
			float oc_mag_sq = oc.SqrdMag();

			float radius = tmax + sqrtf(oc_mag_sq);
			hit_data->p = r.PointAt(tmax);
			hit_data->n = r.PointAt(tmax) / radius;
			hit_data->t = tmax;
		}

		return true;
	}
};

class  solid_plane : public solidhit {
public:
	vec4 v;
	u16 mat = 0;

	solid_plane(vec4 vec) : v(vec), mat(0) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_PLANE; }

	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override {
		const vec3 pnorm = v.xyz();
		float denom = pnorm.Dot(r.dir());

		if (fabsf(denom) <= 0.000001f) return false;

		const vec3 plane0 = pnorm * v.w;
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
};

class  solid_disc : public solidhit
{
public:
	vec4 v;
	float r2;
	u16 mat = 0;

	solid_disc(vec4 vec, float rad) : v(vec), r2(rad*rad), mat(0) {}

	inline u16 get_material() { return mat; }
	inline void set_material(u16 m) { mat = m; }
	inline solid_type get_type() { return SOLID_PLANE; }


	bool hit(const ray3 &r, float tmin, float tmax, hit_record* hit_data) const override {
		const vec3 pnorm = v.xyz();
		float denom = pnorm.Dot(r.dir());

		if (fabsf(denom) <= 0.000001f) return false;

		const vec3 plane0 = v.xyz() * v.w;
		const vec3 p0l0 = plane0 - r.origin();
		const float t = p0l0.Dot(pnorm) / denom;

		if ((t < tmin) || (t > tmax)) return false;

		const vec3 p = r.PointAt(t);
		const float dist2 = (p - plane0).SqrdMag();

		if (dist2 > r2) return false;

		if (hit_data != nullptr) {

			hit_data->t = t;
			hit_data->p = p;
			hit_data->n = pnorm;
		}

		return true;
	}

};

class world {
public:
	material** list_materials = nullptr;
	solidhit** list_solids = nullptr;
	u64 num_solids = 0;
	u16 num_materials = 0;

	void free_world() {
		if (list_materials != nullptr) {
			for (u16 i = num_materials; i >= 0; --i)
				if (list_materials[i]) 
					delete list_materials[i];

			delete[] list_materials;
		}

		if (list_solids != nullptr) {
			for (u64 i = num_solids; i >= 0; --i)
				if (list_solids[i])
					delete list_solids[i];

			delete[] list_solids;
		}

		num_solids = 0;
		num_materials = 0;
	}

	bool hit(const ray3 &r, float tmin, float tmax, hit_record& hit_data) const  {
		hit_record test_hit;
		bool is_hit = false;

		tmax *= 0.9f; // Degrade Fast

		for (int i = 0; i < num_solids; ++i) {
			if (false == list_solids[i]->hit(r, tmin, tmax, &test_hit))
				continue;

			if (is_hit && (test_hit.t > hit_data.t)) continue;

			hit_data = test_hit;
			hit_data.hitid = i;
			tmax = hit_data.t;
			is_hit = true;
		}

		return is_hit;
	}

	bool colour(ray3& ray, const hit_record& hit, vec3& out_colour) const {
		if (hit.hitid > num_solids) return true;
		solidhit* sh = list_solids[hit.hitid];
		u16 mat_index = sh->get_material();
		if (mat_index > num_materials) return true;
		material* mat = list_materials[mat_index];

		return mat->colour(ray, hit, out_colour);
	}

	vec3 cast_ray(const raycast_state& rs, ray3 r, float tmin, float tmax) const {
		hit_record hit_data;
		vec3 out_col = vec3(0.0f, 0.0f, 0.0f);
		for (int i = 0; i < rs.num_bounces; ++i) {
			if (false == hit(r, tmin, tmax, hit_data))
				return out_col;
			
			if (colour(r, hit_data, out_col))
				return out_col;

			// Mix
		}

		return out_col;
	}
};

world g_world;
const world& getWorld() { return g_world; }

void init_world() {
	std::vector<solidhit*> vsolids;
	std::vector<material*> vmats;

	vmats.push_back(new mat_sky({
		vec4(1.0f, 1.0f, 1.0f, 0.0f),
		vec4(0.5f, 0.7f, 1.0f, 0.7f),
		vec4(0.7f, 0.7f, 1.0f, 0.9f) }));
	vsolids.push_back(new solid_skysphere(1000.0f));
	vsolids.back()->set_material(u16(vmats.size() - 1));

	vmats.push_back(new mat_tiles(0.0f, 1.0f));
	vsolids.push_back(new solid_plane(vec4(0.0f, 0.0f, 1.0f, 0.0f)));
	vsolids.back()->set_material(u16(vmats.size() - 1));

	vmats.push_back(new mat_diffuse(0.2f, 1.0f));
	for (vec4 p : {
		vec4(-3.0f, 10.0f, 2.0f, 0.5f),
		vec4(0.0f, 10.0f, 1.0f, 0.5f),
		vec4(+2.7f, 10.5f, 0.7f, 0.5f),
		vec4(+2.7f, 10.5f, 1.7f, 0.5f),
		vec4(+4.0f, 10.0f, 1.0f, 0.9f)
		}) {
		vsolids.push_back(new solid_ball(p));
		vsolids.back()->set_material(u16(vmats.size() - 1));
	}

	vmats.push_back(new mat_normal());
	vsolids[3]->set_material(u16(vmats.size() - 1));

	vmats.push_back(new mat_glass(0.0f, 1.0f));
	vsolids[6]->set_material(u16(vmats.size() - 1));

	//
	g_world.free_world();

	g_world.num_solids = vsolids.size();
	g_world.list_solids = new solidhit*[g_world.num_solids];
	for (u64 i = 0; i < g_world.num_solids; ++i)
		g_world.list_solids[i] = vsolids[i];

	g_world.num_materials = u16(vmats.size());
	g_world.list_materials = new material*[g_world.num_materials];
	for (u16 i = 0; i < g_world.num_materials; ++i)
		g_world.list_materials[i] = vmats[i];
	
	
}

void Raycast(ImageData &output, const raycast_state& rs) {

  const float invWidth = 1.0f / float(output.width);
  const float invHeight = 1.0f / float(output.height);
  const float ratio = float(output.width) / float(output.height);
  const int limit = output.width * output.height * 3;

  const float inv_ns = 1.0f / float(rs.num_samples);
  const vec3 perp = rs.up * rs.cam.dir();

  // Fake Animating world Bullshit which should be in update
  if (g_world.num_solids == 0) 
	  init_world();

  ((solid_ball*)(g_world.list_solids[2]))->v.w = 0.25f + (sinf(g_ticks_since_start * 0.001f) + 1.0f) * 1.0f;
  ((solid_ball*)(g_world.list_solids[3]))->v.z = sinf(g_ticks_since_start * 0.001f) * 2.0f;

  const world& w = getWorld();
  int pass_tick = (rs.pass_break>0)?(rand() % rs.pass_break):0;

  int c = 0;
  for (int y = output.height - 1; y >= 0; --y) {
    float v = (output.height - y) * invHeight;
	for (int x = output.width - 1; x >= 0; --x) {
		if (rs.pass_break > 1) {
			if (--pass_tick > 0) {
				c += 3;
				continue;
			}
			else {
				pass_tick = rand() % rs.pass_break;
			}
		}

		float u = x * invWidth;
		vec3 col(0.0f, 0.0f, 0.0f);
		ray3 r(rs.cam.origin(), rs.cam.dir() + rs.up * (v - 0.5f) + perp * (u - 0.5f) * ratio);
		r.b.Normalize();


		col = w.cast_ray(rs, r, 0.000001f, 300.0f);

		// Sub Sampling
		for (int n = 1; n < rs.num_samples; ++n) {
			const vec2& poi = (rs.num_samples > 8) ? poissonDisc50(n) : poissonDisc8(n);
			ray3 r_sub = ray3(r.a, r.b
				+ perp * poi.x*invWidth
				+ rs.up * poi.y*invHeight);

			col += w.cast_ray(rs, r_sub, 0.000001f, 300.0f);
		}

		if (rs.num_samples > 1)
			col *= inv_ns;

		output.imgData[c++] = char(255 * col.x);
		output.imgData[c++] = char(255 * col.y);
		output.imgData[c++] = char(255 * col.z);
	}
  }

  assert(c <= limit);

  if (rs.border) {
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