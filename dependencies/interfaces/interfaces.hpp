#pragma once
#include <windows.h>
#include <cstdint>
#include <string>
#include <stdexcept>
#include "i_base_client_dll.hpp"
#include "i_client_entity_list.hpp"
#include "iv_engine_client.hpp"
#include "i_client_state.hpp"
#include "i_panel.hpp"
#include "i_surface.hpp"
#include "c_global_vars_base.hpp"
#include "i_material_system.hpp"
#include "iv_model_info.hpp"
#include "iv_model_render.hpp"
#include "iv_debug_overlay.hpp"
#include "i_console.hpp"
#include "i_localize.hpp"
#include "i_game_event_manager.hpp"
#include "i_input.hpp"
#include "i_input_system.hpp"
#include "i_trace.hpp"
#include "i_render_view.hpp"
#include "glow_manager.hpp"
#include "i_player_movement.hpp"
#include "i_weapon_system.hpp"
#include "c_particle_collection.hpp"
#include "i_filesystem.hpp"

namespace interfaces {
	enum class interface_type { index, bruteforce };

	template <typename ret, interface_type type>
	ret* get_interface(const std::string& module_name, const std::string& interface_name) {
		using create_interface_fn = void* (*)(const char*, int*);
		const auto fn = reinterpret_cast<create_interface_fn>(GetProcAddress(GetModuleHandle(module_name.c_str()), "CreateInterface"));

		if (fn) {
			void* result = nullptr;

			switch (type) {
			case interface_type::index:
				result = fn(interface_name.c_str(), nullptr);

				break;
			case interface_type::bruteforce:
				char buf[128];

				for (uint32_t i = 0; i <= 100; ++i) {
					memset(static_cast<void*>(buf), 0, sizeof buf);
					result = fn(interface_name.c_str(), nullptr);
					if (result)
						break;
				}

				break;
			}
		
			if (!result)
				throw std::runtime_error( interface_name + " wasn't found in " + module_name );

			return static_cast<ret*>(result);
		}

		throw std::runtime_error( module_name + " wasn't found" );
	}

	inline i_base_client_dll* client= nullptr;
	inline i_input* input= nullptr;
	inline i_client_entity_list* entity_list= nullptr;
	inline iv_engine_client* engine= nullptr;
	inline i_client_mode* client_mode= nullptr;
	inline i_client_state* client_state= nullptr;
	inline i_panel* panel= nullptr;
	inline i_surface* surface= nullptr;
	inline c_global_vars_base* globals= nullptr;
	inline i_material_system* material_system= nullptr;
	inline iv_model_info* model_info= nullptr;
	inline iv_model_render* model_render= nullptr;
	inline i_render_view* render_view= nullptr;
	inline iv_debug_overlay* debug_overlay= nullptr;
	inline i_console* console= nullptr;
	inline i_localize* localize= nullptr;
	inline i_game_event_manager2* event_manager= nullptr;
	inline i_inputsytem* inputsystem= nullptr;
	inline trace* trace_ray= nullptr;
	inline glow_manager_t* glow_manager= nullptr;
	inline player_game_movement* game_movement= nullptr;
	inline player_prediction* prediction= nullptr;
	inline player_move_helper* move_helper= nullptr;
	inline i_weapon_system* weapon_system= nullptr;
	inline void* key_values_system = nullptr;
	inline i_filesystem* file_system = nullptr;
	inline std::uint8_t* alloc_key_values_client = nullptr;
	inline std::uint8_t* alloc_key_values_engine = nullptr;
	inline std::uintptr_t draw_screen_effect_material;
	inline std::uint8_t* is_depth_of_field_enabled = nullptr;
	inline IDirect3DDevice9* device{};
	inline void* sound = nullptr;
	inline std::uint8_t* line_go_thru_smoke = nullptr;
	bool initialize();
}
