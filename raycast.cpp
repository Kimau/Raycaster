#include "core.h"
#include "gltex.h"
#include "math/algebra.h"
#include "program.h"
#include <stdlib.h>
#include <vector>

#include "raycast.h"
#include "world.h"

typedef void(*cast_ray)(const raycast_state &rs, const world &w, ray3 &r, const vec3 perp, const float invWidth, const float invHeight, vec3& out_col);

void cast_ray_recurse_poisson(const raycast_state &rs, const world &w, ray3 &r, const vec3 perp, const float invWidth, const float invHeight, vec3& out_col) {
	out_col = w.colour(rs, r, 0.000001f, 300.0f, 0);
	for (int n = 1; n < rs.num_samples; ++n) {
		const vec2& poi = (rs.num_samples > 8) ? poissonDisc50(n) : poissonDisc8(n);
		ray3 r_sub = ray3(r.a, r.b
			+ perp * poi.x*invWidth
			+ rs.up * poi.y*invHeight);

		out_col += w.colour(rs, r_sub, 0.000001f, 300.0f, 0);
	}
}

void cast_ray_recurse_random(const raycast_state &rs, const world &w, ray3 &r, const vec3 perp, const float invWidth, const float invHeight, vec3& out_col) {
	out_col = w.colour(rs, r, 0.000001f, 300.0f, 0);
	for (int n = 1; n < rs.num_samples; ++n) {
		ray3 r_sub = ray3(r.a, r.b
			+ perp * (random_float()*2.0f - 1.0f) *invWidth
			+ rs.up * (random_float()*2.0f - 1.0f)*invHeight);

		out_col += w.colour(rs, r_sub, 0.000001f, 300.0f, 0);
	}
}

void cast_ray_flat_poisson(const raycast_state &rs, const world &w, ray3 &r, const vec3 perp, const float invWidth, const float invHeight, vec3& out_col) {
	out_col = w.colour_norecurse(rs, r, 0.000001f, 300.0f);
	for (int n = 1; n < rs.num_samples; ++n) {
		const vec2& poi = (rs.num_samples > 8) ? poissonDisc50(n) : poissonDisc8(n);
		ray3 r_sub = ray3(r.a, r.b
			+ perp * poi.x*invWidth
			+ rs.up * poi.y*invHeight);

		out_col += w.colour_norecurse(rs, r_sub, 0.000001f, 300.0f);
	}
}

void cast_ray_flat_random(const raycast_state &rs, const world &w, ray3 &r, const vec3 perp, const float invWidth, const float invHeight, vec3& out_col) {
	out_col = w.colour_norecurse(rs, r, 0.000001f, 300.0f);
	for (int n = 1; n < rs.num_samples; ++n) {
		ray3 r_sub = ray3(r.a, r.b
			+ perp * (random_float()*2.0f - 1.0f) *invWidth
			+ rs.up * (random_float()*2.0f - 1.0f)*invHeight);

		out_col += w.colour_norecurse(rs, r_sub, 0.000001f, 300.0f);
	}
}

void Raycast(ImageData &output, const raycast_state& rs) {

	const float invWidth = 1.0f / float(output.width);
	const float invHeight = 1.0f / float(output.height);
	const float ratio = float(output.width) / float(output.height);
	const int limit = output.width * output.height * 3;

	const float inv_ns = 1.0f / float(rs.num_samples);
	const vec3 perp = cross(rs.up, rs.cam.b);

	const world& w = getWorld();

	// Pick your raycasting func
	cast_ray casting_func = &cast_ray_flat_poisson;
	if (rs.use_recurse) {
		if (rs.use_poisson)
			casting_func = &cast_ray_recurse_poisson;
		else
			casting_func = &cast_ray_recurse_random;
	}
	else {
		if (rs.use_poisson)
			casting_func = &cast_ray_flat_poisson;
		else
			casting_func = &cast_ray_flat_random;
	}

	u64 t = 0;
	int c = 0;

	switch (rs.drawing) {
	case DRAW_FULL_FRAME:
		for (int y = output.height - 1; y >= 0; --y) {
			float v = (output.height - y) * invHeight;
			for (int x = output.width - 1; x >= 0; --x) {

				float u = x * invWidth;

				vec3 col(0.0f, 0.0f, 0.0f);
				ray3 r(rs.cam.a, rs.cam.b + rs.up * (v - 0.5f) + perp * (u - 0.5f) * ratio);
				r.b.Normalize();

				if (rs.debug == DEBUG_TIME)
					t = SDL_GetPerformanceCounter();

				casting_func(rs, w, r, perp, invWidth, invHeight, col);

				if (rs.num_samples > 1)
					col *= inv_ns;

				if (rs.debug == DEBUG_TIME) {
					t = SDL_GetPerformanceCounter() - t;
					output.imgData[c++] = u8(255 * (sinf(t*0.1f) * 0.5f + 0.5f));
					output.imgData[c++] = u8(255 * (sinf(t*0.01f) * 0.5f + 0.5f));
					output.imgData[c++] = u8(255 * (sinf(t*0.001f) * 0.5f + 0.5f));
					continue;
				}

				output.imgData[c++] = char(255 * col.x);
				output.imgData[c++] = char(255 * col.y);
				output.imgData[c++] = char(255 * col.z);
			}
		} break;

	case DRAW_ONE_LINE: {
		for (int y = output.height - 1; y >= 0; --y) {
			if ((y % rs.num_passes) != rs.curr_pass) {
				c += output.width * 3;
				continue;
			}

			float v = (output.height - y) * invHeight;
			for (int x = output.width - 1; x >= 0; --x) {

				float u = x * invWidth;

				vec3 col(0.0f, 0.0f, 0.0f);
				ray3 r(rs.cam.a, rs.cam.b + rs.up * (v - 0.5f) + perp * (u - 0.5f) * ratio);
				r.b.Normalize();

				if (rs.debug == DEBUG_TIME)
					t = SDL_GetPerformanceCounter();

				casting_func(rs, w, r, perp, invWidth, invHeight, col);

				if (rs.num_samples > 1)
					col *= inv_ns;

				if (c >= (limit - 3))
					c = 0;

				if (rs.debug == DEBUG_TIME) {
					t = SDL_GetPerformanceCounter() - t;
					output.imgData[c++] = u8(255 * (sinf(t*0.1f) * 0.5f + 0.5f));
					output.imgData[c++] = u8(255 * (sinf(t*0.01f) * 0.5f + 0.5f));
					output.imgData[c++] = u8(255 * (sinf(t*0.001f) * 0.5f + 0.5f));
				}
				else {
					output.imgData[c++] = char(255 * col.x);
					output.imgData[c++] = char(255 * col.y);
					output.imgData[c++] = char(255 * col.z);
				}
			}
		}

		if (c >= (limit - 3))
			c = 0;
	} break;

	case DRAW_RANDOM: {
		int y = output.height - 1;
		int x = output.width - 1;

		if (rs.num_passes <= 1) break;

		while(y > 0) {
			float v = (output.height - y) * invHeight;
			float u = x * invWidth;

			vec3 col(0.0f, 0.0f, 0.0f);
			ray3 r(rs.cam.a, rs.cam.b + rs.up * (v - 0.5f) + perp * (u - 0.5f) * ratio);
			r.b.Normalize();

			if (rs.debug == DEBUG_TIME)
				t = SDL_GetPerformanceCounter();

			casting_func(rs, w, r, perp, invWidth, invHeight, col);

			if (rs.num_samples > 1)
				col *= inv_ns;

			if (rs.debug == DEBUG_TIME) {
				t = SDL_GetPerformanceCounter() - t;
				output.imgData[c++] = u8(255 * (sinf(t*0.1f) * 0.5f + 0.5f));
				output.imgData[c++] = u8(255 * (sinf(t*0.01f) * 0.5f + 0.5f));
				output.imgData[c++] = u8(255 * (sinf(t*0.001f) * 0.5f + 0.5f));
				continue;
			}

			output.imgData[c++] = char(255 * col.x);
			output.imgData[c++] = char(255 * col.y);
			output.imgData[c++] = char(255 * col.z);
			x--;


			int cdown = (rand() % (rs.num_passes - 1));
			c += cdown * 3;
			if (c >= limit) {
				c = limit-3;
				break;
			}

			x -= cdown;
			while (x < 0) {
				y -= 1;
				x += output.width;
			}
		}
	} break;
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