#pragma once

namespace hooks {
	bool initialize();
	void release();
	//imgui stuffs?
	extern WNDPROC wndproc_original;
	extern HWND window;
	LRESULT __stdcall wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam);

	namespace present {
		static constexpr auto index = 17;
		static HRESULT D3DAPI hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region);
		using fn = long(__stdcall*)(IDirect3DDevice9*, RECT*, RECT*, HWND, RGNDATA*);
		inline fn original = nullptr;
	}

	namespace reset {
		static constexpr auto index = 16;
		using fn = long(__stdcall*)(IDirect3DDevice9*, D3DPRESENT_PARAMETERS*);
		long __stdcall hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters);
		inline fn original = nullptr;
	}

	inline unsigned int get_virtual(void* _class, unsigned int index) {
		return static_cast<unsigned int>((*static_cast<int**>(_class))[index]);
	}

	namespace create_move {
		static constexpr auto index = 24;
		using fn = bool(__thiscall*)(i_client_mode*, float, c_usercmd*);
		inline fn original = nullptr;
		bool __stdcall hook(float input_sample_frametime, c_usercmd* cmd) noexcept;
	};

	namespace paint_traverse {
		static constexpr auto index = 41;
		using fn = void(__thiscall*)(i_panel*, unsigned int, bool, bool);
		inline fn original = nullptr;
		void __stdcall hook(unsigned int panel, bool force_repaint, bool allow_force) noexcept;
	}

	namespace on_screen_size_changed {
		static constexpr auto index = 116;
		using fn = void(__thiscall*)(int, int);
		inline fn original = nullptr;
		void __stdcall hook(int oldWidth, int oldHeight);
	}

	namespace override_view {
		static constexpr auto index = 18;
		using fn = void* (__fastcall*)(i_client_mode*, void* _this, view_setup_t* setup);
		inline fn original = nullptr;
		static void __fastcall hook(void* _this, void* _edx, view_setup_t* setup);
	}

	namespace draw_model_execute {
		static constexpr auto index = 21;
		using fn = void(__thiscall*)(iv_model_render*, i_material_render_context*, const draw_model_state_t&, const model_render_info_t&, matrix_t*);
		inline fn original = nullptr;
		static void __stdcall hook(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world);
	}

	namespace frame_stage_notify {
		static constexpr auto index = 37;
		using fn = void(__thiscall*)(i_base_client_dll*, int);
		inline fn original = nullptr;
		static void __stdcall hook(int frame_stage);
	}

	namespace do_post_screen_effects {
		static constexpr auto index = 44;
		using fn = int(__thiscall*)(void*, int);
		inline fn original = nullptr;
		static int __stdcall hook(int value);
	}

	namespace sv_cheats_get_bool {
		static constexpr auto index = 13;
		using fn = bool(__thiscall*)(void*);
		inline fn original = nullptr;
		static bool __fastcall hook(PVOID convar, int edx);
	}

	namespace list_leaves_in_box {
		static constexpr auto index = 6;
		using fn = int(__thiscall*)(void*, const vec3_t&, const vec3_t&, unsigned short*, int);
		inline fn original = nullptr;
		static int __fastcall hook(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max);
	}

	namespace get_color_modulation {
		using fn = void(__thiscall*)(void*, float*, float*, float*);
		inline fn original = nullptr;
		static void __fastcall hook(void* ecx, void* edx, float* r, float* g, float* b);
	}

	namespace is_using_static_prop_debug_modes {
		using fn = bool(__stdcall*)();
		inline fn original = nullptr;
		static bool __stdcall hook();
	}

	namespace is_depth_of_field_enabled {
		static bool __stdcall hook();
	}

	namespace alloc_key_value_memory {
		static constexpr auto index = 1;
		using fn = void* (__thiscall*)(void*, const std::int32_t);
		inline fn original = nullptr;
		void* __stdcall hook(const std::int32_t size) noexcept;
	}

	namespace check_file_CRCs_with_server {
		void __fastcall hook(void* edc, void* edx);
	}

	namespace loose_file_allowed {
		static constexpr auto index = 128;
		bool __fastcall hook(void* edc, void* edx);
	}

	namespace get_unverified_file_hashes {
		static constexpr auto index = 101;
		int __stdcall hook(void* _this, void* some_class, int max_files);
	}

	namespace particle_collection_simulate {
		using fn = void(__thiscall*)(particle_collection*);
		inline fn original = nullptr;
		void __fastcall hook(particle_collection* this_ptr, void* edx);
	}
}
