/*
 * Modern effects for a modern Streamer
 * Copyright (C) 2019 Michael Fabian Dirks
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA
 */

#pragma once
#include "common.hpp"
#include "gs-sampler.hpp"
#include "gs-texture.hpp"

namespace streamfx::obs::gs {
	class effect_parameter : public std::shared_ptr<gs_eparam_t> {
		std::shared_ptr<gs_effect_t> _effect_parent;
		std::shared_ptr<gs_epass_t>  _pass_parent;
		std::shared_ptr<gs_eparam_t> _param_parent;

		public:
		enum class type {
			Unknown,
			Boolean,
			Float,
			Float2,
			Float3,
			Float4,
			Integer,
			Integer2,
			Integer3,
			Integer4,
			Matrix,
			String,
			Texture,

			Invalid = -1,
		};

		public:
		effect_parameter();
		effect_parameter(gs_eparam_t* param);
		effect_parameter(gs_eparam_t* param, std::shared_ptr<gs_effect_t> parent);
		effect_parameter(gs_eparam_t* param, std::shared_ptr<gs_epass_t> parent);
		effect_parameter(gs_eparam_t* param, std::shared_ptr<gs_eparam_t> parent);
		~effect_parameter();

		effect_parameter(const effect_parameter& rhs);
		effect_parameter& operator=(const effect_parameter& rhs);

		effect_parameter(effect_parameter&& rhs) noexcept;
		effect_parameter& operator=(effect_parameter&& rhs) noexcept;

		std::string_view get_name();

		type get_type();

		std::size_t      count_annotations();
		effect_parameter get_annotation(std::size_t idx);
		effect_parameter get_annotation(const std::string_view name);
		bool             has_annotation(const std::string_view name);
		bool             has_annotation(const std::string_view name, effect_parameter::type type);

		public /* Memory API */:
		std::size_t get_default_value_size_in_bytes()
		{
			return gs_effect_get_default_val_size(get());
		}

		template<typename T>
		std::size_t get_default_value_size()
		{
			return gs_effect_get_default_val_size(get()) / sizeof(T);
		}

		template<typename T>
		bool get_default_value(T v[], std::size_t len)
		{
			if (len != get_default_value_size<T>()) {
				return false;
			}

			if (T* ptr = reinterpret_cast<T*>(gs_effect_get_default_val(get())); ptr != nullptr) {
				for (std::size_t idx = 0; idx < len; idx++) {
					v[idx] = *(ptr + idx);
				}

				bfree(ptr);
				return true;
			}
			return false;
		}

		std::size_t get_value_size_in_bytes()
		{
			return gs_effect_get_val_size(get());
		}

		template<typename T>
		std::size_t get_value_size()
		{
			return gs_effect_get_val_size(get()) / sizeof(T);
		}

		template<typename T>
		bool get_value(T v[], std::size_t len)
		{
			if (len != get_value_size<T>()) {
				return false;
			}

			if (T* ptr = reinterpret_cast<T*>(gs_effect_get_val(get())); ptr != nullptr) {
				for (std::size_t idx = 0; idx < len; idx++) {
					v[idx] = *(ptr + idx);
				}

				bfree(ptr);
				return true;
			}
			return false;
		}

		template<typename T>
		bool set_value(T v[], std::size_t len)
		{
			gs_effect_set_val(get(), v, sizeof(T) * len);
			return true;
		}

		public /* Value API */:
		void set_bool(bool v);
		void get_bool(bool& v);
		void get_default_bool(bool& v);

		void set_bool_array(bool v[], std::size_t sz);

		void set_float(float_t x);
		void get_float(float_t& x);
		void get_default_float(float_t& x);

		void set_float2(vec2 const& v);
		void get_float2(vec2& v);
		void get_default_float2(vec2& v);
		void set_float2(float_t x, float_t y);
		void get_float2(float_t& x, float_t& y);
		void get_default_float2(float_t& x, float_t& y);

		void set_float3(vec3 const& v);
		void get_float3(vec3& v);
		void get_default_float3(vec3& v);
		void set_float3(float_t x, float_t y, float_t z);
		void get_float3(float_t& x, float_t& y, float_t& z);
		void get_default_float3(float_t& x, float_t& y, float_t& z);

		void set_float4(vec4 const& v);
		void get_float4(vec4& v);
		void get_default_float4(vec4& v);
		void set_float4(float_t x, float_t y, float_t z, float_t w);
		void get_float4(float_t& x, float_t& y, float_t& z, float_t& w);
		void get_default_float4(float_t& x, float_t& y, float_t& z, float_t& w);

		void set_int(int32_t x);
		void get_int(int32_t& x);
		void get_default_int(int32_t& x);

		void set_int2(int32_t x, int32_t y);
		void get_int2(int32_t& x, int32_t& y);
		void get_default_int2(int32_t& x, int32_t& y);

		void set_int3(int32_t x, int32_t y, int32_t z);
		void get_int3(int32_t& x, int32_t& y, int32_t& z);
		void get_default_int3(int32_t& x, int32_t& y, int32_t& z);

		void set_int4(int32_t x, int32_t y, int32_t z, int32_t w);
		void get_int4(int32_t& x, int32_t& y, int32_t& z, int32_t& w);
		void get_default_int4(int32_t& x, int32_t& y, int32_t& z, int32_t& w);

		void set_matrix(matrix4 const& v);
		void get_matrix(matrix4& v);
		void get_default_matrix(matrix4& v);

		void set_texture(std::shared_ptr<streamfx::obs::gs::texture> v);
		void set_texture(gs_texture_t* v);

		void set_sampler(std::shared_ptr<streamfx::obs::gs::sampler> v);
		void set_sampler(gs_sampler_state* v);

		void set_string(std::string const& v);
		void get_string(std::string& v);
		void get_default_string(std::string& v);

		public /* Helpers */:
		inline float_t get_bool()
		{
			bool v;
			get_bool(v);
			return v;
		};
		inline bool get_default_bool()
		{
			bool v;
			get_default_bool(v);
			return v;
		};

		inline float_t get_float()
		{
			float_t v;
			get_float(v);
			return v;
		};
		inline float_t get_default_float()
		{
			float_t v;
			get_default_float(v);
			return v;
		};

		inline vec2 get_float2()
		{
			vec2 v;
			get_float2(v);
			return v;
		};
		inline vec2 get_default_float2()
		{
			vec2 v;
			get_default_float2(v);
			return v;
		};

		inline vec3 get_float3()
		{
			vec3 v;
			get_float3(v);
			return v;
		};
		inline vec3 get_default_float3()
		{
			vec3 v;
			get_default_float3(v);
			return v;
		};

		inline vec4 get_float4()
		{
			vec4 v;
			get_float4(v);
			return v;
		};
		inline vec4 get_default_float4()
		{
			vec4 v;
			get_default_float4(v);
			return v;
		};

		inline int32_t get_int()
		{
			int32_t v;
			get_int(v);
			return v;
		};
		inline int32_t get_default_int()
		{
			int32_t v;
			get_default_int(v);
			return v;
		};

		inline matrix4 get_matrix()
		{
			matrix4 v;
			get_matrix(v);
			return v;
		};
		inline matrix4 get_default_matrix()
		{
			matrix4 v;
			get_default_matrix(v);
			return v;
		};

		inline std::string get_string()
		{
			std::string v;
			get_string(v);
			return v;
		};
		inline std::string get_default_string()
		{
			std::string v;
			get_default_string(v);
			return v;
		};
	};
} // namespace streamfx::obs::gs
