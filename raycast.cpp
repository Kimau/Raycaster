#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include "program.h"
#include <stdlib.h>
#include <vector>

#include "hit_record.h"
#include "material.h"
#include "solid_hit.h"

// Z Up


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

class world;
class hit_record;
class material;
class solidhit;


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

	const material* lookup_material(const hit_record& hit) const {
		if (hit.hitid > num_solids) return nullptr;
		solidhit* sh = list_solids[hit.hitid];
		u16 mat_index = sh->get_material();
		if (mat_index > num_materials) return nullptr;
		return list_materials[mat_index];
	}

	vec3 cast_ray(const raycast_state& rs, ray3 r, float tmin, float tmax) const {
		hit_record hit_data;
		vec3 out_col = vec3(0.0f, 0.0f, 0.0f);

		u16 bounce_index = 0;
		const material* mat_bounch_backs[100];
		mat_bounch_backs[0] = nullptr;

		int i = 0;
		while(i < rs.num_bounces) {
			if (false == hit(r, tmin, tmax, hit_data))
				break;

			const material* mat = lookup_material(hit_data);
			if (mat == nullptr)
				break;

			if ((bounce_index<100) && (mat->needsBounceBack()))
				mat_bounch_backs[bounce_index++] = mat;
			
			if(mat->colour(r, hit_data, out_col))
				break;

			++i;
		}

		while (bounce_index > 0) {
			mat_bounch_backs[--bounce_index]->bounceBack(out_col);
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
		vec4(0.2f, 0.2f, 0.2f,-0.9f),
		vec4(1.0f, 0.8f, 0.8f,-0.3f),
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

	vmats.push_back(new mat_diffuse(0.5f, 0.5f));
	vsolids[4]->set_material(u16(vmats.size() - 1));
	vsolids[5]->set_material(u16(vmats.size() - 1));

	vmats.push_back(new mat_glass(0.0f, vec3(1.0f, 1.0f, 1.0f)));
	vsolids[6]->set_material(u16(vmats.size() - 1));


	vsolids.push_back(new solid_ball(vec4(-5.0f, 5.0f, 3.0f, 2.0f)));
	vmats.push_back(new mat_lambertian(vec3(1.0f, 0.0f, 0.0f), 0.1f));
	vsolids.back()->set_material(u16(vmats.size() - 1));

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
		if (rs.use_poisson) {
			for (int n = 1; n < rs.num_samples; ++n) {
				const vec2& poi = (rs.num_samples > 8) ? poissonDisc50(n) : poissonDisc8(n);
				ray3 r_sub = ray3(r.a, r.b
					+ perp * poi.x*invWidth
					+ rs.up * poi.y*invHeight);

				col += w.cast_ray(rs, r_sub, 0.000001f, 300.0f);
			}
		}
		else { // C Random
			for (int n = 1; n < rs.num_samples; ++n) {
				ray3 r_sub = ray3(r.a, r.b
					+ perp * (random_float()*2.0f-1.0f) *invWidth
					+ rs.up * (random_float()*2.0f - 1.0f)*invHeight);

				col += w.cast_ray(rs, r_sub, 0.000001f, 300.0f);
			}
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