#include "../../dependencies/utilities/csgo.hpp"
#include "../features/features.hpp"
#include "../features/misc/engine_prediction.hpp"
#include "../menu/menu.hpp"
#include "../../dependencies/utilities/post_processing/post_processing.h"

HWND hooks::window;
WNDPROC hooks::wndproc_original = NULL;
float alpha = 0.f;
float animation_frequency = 0.75f;

bool hooks::initialize() {
	auto present_target = reinterpret_cast<void*>(get_virtual(interfaces::device, present::index));
	auto reset_target = reinterpret_cast<void*>(get_virtual(interfaces::device, reset::index));

	const auto create_move_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, create_move::index));
	const auto paint_traverse_target = reinterpret_cast<void*>(get_virtual(interfaces::panel, paint_traverse::index));
	const auto on_screen_size_changed_target = reinterpret_cast<void*>(get_virtual(interfaces::surface, on_screen_size_changed::index));
	const auto override_view_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, override_view::index));
	const auto draw_model_execute_target = reinterpret_cast<void*>(get_virtual(interfaces::model_render, draw_model_execute::index));
	const auto frame_stage_notify_target = reinterpret_cast<void*>(get_virtual(interfaces::client, frame_stage_notify::index));
	const auto sv_cheats_get_bool_target = reinterpret_cast<void*>(get_virtual(interfaces::console->get_convar(("sv_cheats")), sv_cheats_get_bool::index));
	const auto do_post_screen_effects_target = reinterpret_cast<void*>(get_virtual(interfaces::client_mode, do_post_screen_effects::index));
	const auto get_color_modulation_target = reinterpret_cast<void*>(utilities::pattern_scan(("materialsystem.dll"), sig_color_modulation));
	const auto list_leaves_in_box_target = reinterpret_cast<void*>(get_virtual(interfaces::engine->get_bsp_query(), list_leaves_in_box::index));
	const auto is_using_static_prop_debug_modes_target = reinterpret_cast<void*>(utilities::pattern_scan(("engine.dll"), sig_is_using_static_prop_debug_modes));
	const auto is_depth_of_field_enabled_target = reinterpret_cast<void*>(interfaces::is_depth_of_field_enabled);
	const auto alloc_key_value_memory_target = reinterpret_cast<void*>(get_virtual(interfaces::key_values_system, alloc_key_value_memory::index));
	const auto check_file_CRCs_with_server_target = reinterpret_cast<void*>(utilities::pattern_scan(("engine.dll"), sig_check_file_CRCs_with_server));
	const auto loose_file_allowed_target = reinterpret_cast<void*>(get_virtual(interfaces::file_system, loose_file_allowed::index));
	const auto get_unverified_file_hashes_target = reinterpret_cast<void*>(get_virtual(interfaces::file_system, get_unverified_file_hashes::index));
	const auto particle_collection_simulate = reinterpret_cast<void*>(utilities::pattern_scan("client.dll", sig_particle_collection_simulate));

	if (MH_Initialize() != MH_OK)
		throw std::runtime_error(("failed to initialize MH_Initialize."));

	if (MH_CreateHook(present_target, &present::hook, reinterpret_cast<void**>(&present::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize present."));

	if (MH_CreateHook(reset_target, &reset::hook, reinterpret_cast<void**>(&reset::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize reset."));

	if (MH_CreateHook(create_move_target, &create_move::hook, reinterpret_cast<void**>(&create_move::original)) != MH_OK)
		throw std::runtime_error(("failed to initialize create_move."));

	if (MH_CreateHook(paint_traverse_target, &paint_traverse::hook, reinterpret_cast<void**>(&paint_traverse::original)) != MH_OK)
		throw std::runtime_error(("failed to initialize paint_traverse."));

	if (MH_CreateHook(on_screen_size_changed_target, &on_screen_size_changed::hook, reinterpret_cast<void**>(&on_screen_size_changed::original)) != MH_OK)
		throw std::runtime_error(("failed to initialize on_screen_size_changed_target."));

	if (MH_CreateHook(draw_model_execute_target, &draw_model_execute::hook, reinterpret_cast<void**>(&hooks::draw_model_execute::original)) != MH_OK)
		throw std::runtime_error(("failed to initialize draw_model_execute."));

	if (MH_CreateHook(override_view_target, &override_view::hook, reinterpret_cast<void**>(&override_view::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize override_view."));
	
	if (MH_CreateHook(frame_stage_notify_target, &frame_stage_notify::hook, reinterpret_cast<void**>(&frame_stage_notify::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize frame_stage_notify."));

	if (MH_CreateHook(sv_cheats_get_bool_target, &sv_cheats_get_bool::hook, reinterpret_cast<void**>(&sv_cheats_get_bool::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize sv_cheats."));

	if (MH_CreateHook(do_post_screen_effects_target, &hooks::do_post_screen_effects::hook, reinterpret_cast<void**>(&do_post_screen_effects::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize do_post_screen_effects."));

	if (MH_CreateHook(list_leaves_in_box_target, &hooks::list_leaves_in_box::hook, reinterpret_cast<void**>(&list_leaves_in_box::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize list_leaves_in_box."));

	if (MH_CreateHook(get_color_modulation_target, &hooks::get_color_modulation::hook, reinterpret_cast<void**>(&get_color_modulation::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize get_color_modulation."));

	if (MH_CreateHook(is_using_static_prop_debug_modes_target, &hooks::is_using_static_prop_debug_modes::hook, reinterpret_cast<void**>(&is_using_static_prop_debug_modes::original)) != MH_OK) 
		throw std::runtime_error(("failed to initialize is_using_static_prop_debug_modes."));

	if (MH_CreateHook(is_depth_of_field_enabled_target, &is_depth_of_field_enabled::hook, nullptr) != MH_OK)
		throw std::runtime_error(("failed to initialize is_depth_of_field_enabled."));

	if (MH_CreateHook(alloc_key_value_memory_target, &alloc_key_value_memory::hook, reinterpret_cast<void**>(&alloc_key_value_memory::original)) != MH_OK)
		throw std::runtime_error(("failed to initialize alloc_key_value_memory."));

	if (MH_CreateHook(check_file_CRCs_with_server_target, &check_file_CRCs_with_server::hook, nullptr) != MH_OK)
		throw std::runtime_error(("failed to initialize check_file_CRCs_with_server."));

	if (MH_CreateHook(loose_file_allowed_target, &loose_file_allowed::hook, nullptr) != MH_OK)
		throw std::runtime_error(("failed to initialize loose_file_allowed."));

	if (MH_CreateHook(get_unverified_file_hashes_target, &get_unverified_file_hashes::hook, nullptr) != MH_OK)
		throw std::runtime_error(("failed to initialize loose_file_allowed."));

	if (MH_CreateHook(particle_collection_simulate, &particle_collection_simulate::hook, reinterpret_cast<void**>(&particle_collection_simulate::original)) != MH_OK)
		throw std::runtime_error(("failed to initialize particle_collection_simulate."));

	if (MH_EnableHook(MH_ALL_HOOKS) != MH_OK)
		throw std::runtime_error(("failed to enable hooks."));

	window = FindWindowW(L"Valve001", NULL);
	if (!window)
		throw std::runtime_error(("failed to initialize game window."));

	wndproc_original = reinterpret_cast<WNDPROC>(SetWindowLongW(window, GWL_WNDPROC, reinterpret_cast<LONG>(wndproc)));

	console::push_color(FOREGROUND_CYAN);
	console::log("[info]", "hooks initialized!");
	console::pop_color();
	return true;
}

void hooks::release() {
	MH_Uninitialize();
	//interfaces::console->console_color_printf(color::menu(), "Bye");
	MH_DisableHook(MH_ALL_HOOKS);
	MH_RemoveHook(MH_ALL_HOOKS);

	SetWindowLongW(FindWindowW((L"Valve001"), NULL), GWL_WNDPROC, reinterpret_cast<LONG>(wndproc_original));
}

LRESULT __stdcall hooks::wndproc(HWND hwnd, UINT message, WPARAM wparam, LPARAM lparam) {
	if (message == WM_KEYDOWN && LOWORD(wparam) == VK_INSERT)
		menu::settings::open = !menu::settings::open;

	LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
	ImGui_ImplWin32_WndProcHandler(window, message, wparam, lparam);

	return CallWindowProcW(wndproc_original, hwnd, message, wparam, lparam);
}

std::once_flag flag;
static HRESULT D3DAPI hooks::present::hook(IDirect3DDevice9* device, RECT* source_rect, RECT* dest_rect, HWND dest_window_override, RGNDATA* dirty_region) {
	std::call_once(flag, [&] { menu::initialize(); });

	static bool init_imgui{ ImGui_ImplDX9_Init(device) };

	device->SetRenderState(D3DRS_COLORWRITEENABLE, D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE);
	IDirect3DVertexDeclaration9* vertexDeclaration;
	device->GetVertexDeclaration(&vertexDeclaration);

	ImGui_ImplDX9_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

	post_processing::set_device(device);
	post_processing::new_frame();

	int w, h;
	interfaces::engine->get_screen_size(w, h);

	menu::render();
	if (!menu::settings::open){
		menu::settings::alpha = 0;
		alpha = std::clamp(alpha - (animation_frequency * ImGui::GetIO().DeltaTime) * 2, 0.f, 1.f);
		if (variables::bg_blur)
			post_processing::perform_blur(ImGui::GetBackgroundDrawList(), alpha, w, h);
	}


	ImGui::EndFrame();
	ImGui::Render();

	if (device->BeginScene() == D3D_OK) {
		ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
		device->EndScene();
	}
	device->SetVertexDeclaration(vertexDeclaration);
	vertexDeclaration->Release();

	return present::original(device, source_rect, dest_rect, dest_window_override, dirty_region);
}

long __stdcall hooks::reset::hook(IDirect3DDevice9* device, D3DPRESENT_PARAMETERS* present_parameters) {
	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto result = reset::original(device, present_parameters);
	post_processing::on_device_reset();
	ImGui_ImplDX9_CreateDeviceObjects();

	return result;
}


bool __stdcall hooks::create_move::hook(float input_sample_frametime, c_usercmd* cmd) noexcept {
	if (!cmd || !cmd->command_number)
		return create_move::original(interfaces::client_mode, input_sample_frametime, cmd);

	if (create_move::original(interfaces::client_mode, input_sample_frametime, cmd))
		interfaces::engine->set_view_angles(cmd->viewangles);

	csgo::local_player = static_cast<player_t*>(interfaces::entity_list->get_client_entity(interfaces::engine->get_local_player()));

	uintptr_t* frame_pointer;
	__asm mov frame_pointer, ebp;
	bool& send_packet = *reinterpret_cast<bool*>(*frame_pointer - 0x34);

	if (interfaces::client_state->delta_tick > 0)
		interfaces::prediction->update(
			interfaces::client_state->delta_tick, interfaces::client_state->delta_tick > 0,
			interfaces::client_state->last_command_ack, interfaces::client_state->last_outgoing_command + interfaces::client_state->choked_commands
		);

	auto old_viewangles = cmd->viewangles;
	auto old_forwardmove = cmd->forwardmove;
	auto old_sidemove = cmd->sidemove;
	auto old_flags = csgo::local_player->flags();

	misc::movement::bunny_hop(cmd);
	misc::movement::infinite_crouch(cmd);
	misc::movement::null_strafe(cmd);
	misc::movement::auto_pistol(cmd);

	prediction::start(cmd); {


	} prediction::end();

	misc::movement::edge_jump(cmd, old_flags);
	misc::movement::edge_bug(cmd, old_flags);
	misc::movement::jump_bug(cmd, old_flags);
	misc::movement::mini_jump(cmd);
	misc::movement::strafe_optimizer(cmd);

	math::correct_movement(old_viewangles, cmd, old_forwardmove, old_sidemove);

	cmd->forwardmove = std::clamp(cmd->forwardmove, -450.0f, 450.0f);
	cmd->sidemove = std::clamp(cmd->sidemove, -450.0f, 450.0f);
	cmd->upmove = std::clamp(cmd->upmove, -320.0f, 320.0f);

	cmd->viewangles.normalize();
	cmd->viewangles.x = std::clamp(cmd->viewangles.x, -89.0f, 89.0f);
	cmd->viewangles.y = std::clamp(cmd->viewangles.y, -180.0f, 180.0f);
	cmd->viewangles.z = 0.0f;

	return false;
}

void __stdcall hooks::paint_traverse::hook(unsigned int panel, bool force_repaint, bool allow_force) noexcept {
	auto panel_to_draw = fnv::hash(interfaces::panel->get_panel_name(panel));

	switch (panel_to_draw) {
	case fnv::hash("MatSystemTopPanel"):
		//visuals::player::run();
		int w, h;
		interfaces::engine->get_screen_size(w, h);
		if (csgo::local_player && csgo::local_player->is_alive()) {
			int vel = csgo::local_player->velocity().length_2d();
			render::text(w / 2, h / 2 + 400, render::fonts::indicator_font, std::format("Vel: {}", vel), true, color(255, 255, 255, 255));
		}
		break;

	case fnv::hash("FocusOverlayPanel"):
		if (!GetAsyncKeyState(VK_END)) {
			interfaces::panel->set_keyboard_input_enabled(panel, menu::settings::open);
			interfaces::panel->set_mouse_input_enabled(panel, menu::settings::open);
		}

		if (menu::settings::open)
			alpha = std::clamp(alpha + animation_frequency * ImGui::GetIO().DeltaTime, 0.f, 1.f);

		break;
	}

	paint_traverse::original(interfaces::panel, panel, force_repaint, allow_force);
}

void _stdcall hooks::on_screen_size_changed::hook(int oldWidth, int oldHeightt) {
	std::call_once(flag, [&] { menu::initialize(); });

	menu::render();

	on_screen_size_changed::original(oldHeightt, oldWidth);
}

void __fastcall hooks::override_view::hook(void* _this, void* _edx, view_setup_t* setup) {
	override_view::original(interfaces::client_mode, _this, setup);
}

void __stdcall hooks::draw_model_execute::hook(i_material_render_context* ctx, const draw_model_state_t& state, const model_render_info_t& info, matrix_t* bone_to_world) {
	if (interfaces::model_render->is_forced_material_override())
		return draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);

	draw_model_execute::original(interfaces::model_render, ctx, state, info, bone_to_world);
}

void __stdcall hooks::frame_stage_notify::hook(int frame_stage) {
	if (!interfaces::engine->is_in_game()) 
		return frame_stage_notify::original(interfaces::client, frame_stage);

	frame_stage_notify::original(interfaces::client, frame_stage);
}

bool __fastcall hooks::sv_cheats_get_bool::hook(PVOID convar, int edx) {
	static auto cam_think = utilities::pattern_scan(("client.dll"), sig_cam_think);

	if (!convar)
		return false;

	if ((_ReturnAddress()) == cam_think && csgo::local_player && csgo::local_player->is_alive())
		return true;
	else
		return sv_cheats_get_bool::original(convar);
}

int __stdcall hooks::do_post_screen_effects::hook(int value) {

	return do_post_screen_effects::original(interfaces::client_mode, value);
}

#define MAX_COORD_FLOAT ( 16384.0f )
#define MIN_COORD_FLOAT ( -MAX_COORD_FLOAT )
int __fastcall hooks::list_leaves_in_box::hook(void* bsp, void* edx, const vec3_t& mins, const vec3_t& maxs, unsigned short* list, int list_max) {
	static auto list_leaves = utilities::pattern_scan(("client.dll"), sig_list_leaves) + 5;
	auto info = *reinterpret_cast<renderable_info_t**>(reinterpret_cast<uintptr_t>(_AddressOfReturnAddress()) + 0x14);

	if ((_ReturnAddress()) != list_leaves)
		return list_leaves_in_box::original(bsp, mins, maxs, list, list_max);

	if (!info || !info->renderable)
		return list_leaves_in_box::original(bsp, mins, maxs, list, list_max);

	auto entity = utilities::call_virtual_method<entity_t*>(info->renderable - 4, 7);

	if (!entity || entity->client_class()->class_id != class_ids::ccsplayer) //is_player
		return list_leaves_in_box::original(bsp, mins, maxs, list, list_max);

	info->flags &= ~0x100;
	info->flags2 |= 0x40;

	static const vec3_t map_min = vec3_t(MIN_COORD_FLOAT, MIN_COORD_FLOAT, MIN_COORD_FLOAT);
	static const vec3_t map_max = vec3_t(MAX_COORD_FLOAT, MAX_COORD_FLOAT, MAX_COORD_FLOAT);
	return list_leaves_in_box::original(bsp, map_min, map_max, list, list_max);
}

void __fastcall hooks::get_color_modulation::hook(void* ecx, void* edx, float* r, float* g, float* b) {
	get_color_modulation::original(ecx, r, g, b);

	//const auto material = reinterpret_cast<i_material*>(ecx);

	//if (!material || material->is_error_material())
	//	return get_color_modulation::original(ecx, r, g, b);

	//const auto group = fnv::hash(material->get_texture_group_name());

	//if (group != fnv::hash(("World textures")) && group != fnv::hash(("StaticProp textures"))
	//	&& (group != fnv::hash(("SkyBox textures"))))
	//	return get_color_modulation::original(ecx, r, g, b);

	//bool is_prop = (group == fnv::hash(("StaticProp textures")));

	//*r *= is_prop ? 0.45f : 0.15f;
	//*g *= is_prop ? 0.45f : 0.15f;
	//*b *= is_prop ? 0.45f : 0.15f;
}

bool __stdcall hooks::is_using_static_prop_debug_modes::hook() {
	return false;
}

static bool __stdcall hooks::is_depth_of_field_enabled::hook() {
	return false;
}

void* __stdcall hooks::alloc_key_value_memory::hook(const std::int32_t size) noexcept {
	if (const std::uint32_t address = reinterpret_cast<std::uint32_t>(_ReturnAddress());
		address == reinterpret_cast<std::uint32_t>(interfaces::alloc_key_values_engine) ||
		address == reinterpret_cast<std::uint32_t>(interfaces::alloc_key_values_client))
		return nullptr;

	return alloc_key_value_memory::original(interfaces::key_values_system, size);
}

void __fastcall hooks::check_file_CRCs_with_server::hook(void* edc, void* edx) {
	return;
}

bool __fastcall hooks::loose_file_allowed::hook(void* edc, void* edx) {
	return true;
}

int __stdcall hooks::get_unverified_file_hashes::hook(void* _this, void* some_class, int max_files) {
	return 0;
}

void __fastcall hooks::particle_collection_simulate::hook(particle_collection* this_ptr, void* edx) {
	particle_collection_simulate::original(this_ptr);

	particle_collection* root_colection = this_ptr;
	while (root_colection->parent)
		root_colection = root_colection->parent;

	const char* root_name = root_colection->definition.object->name.buffer;

	switch (fnv::hash(root_name)) {
		case fnv::hash("molotov_groundfire"):
		case fnv::hash("molotov_groundfire_00MEDIUM"):
		case fnv::hash("molotov_groundfire_00HIGH"):
		case fnv::hash("molotov_groundfire_fallback"):
		case fnv::hash("molotov_groundfire_fallback2"):
		case fnv::hash("molotov_explosion"):
		case fnv::hash("explosion_molotov_air"):
		case fnv::hash("extinguish_fire"):
		case fnv::hash("weapon_molotov_held"):
		case fnv::hash("weapon_molotov_fp"):
		case fnv::hash("weapon_molotov_thrown"):
		case fnv::hash("incgrenade_thrown_trail"):
			switch (fnv::hash(this_ptr->definition.object->name.buffer))
			{
			case fnv::hash("explosion_molotov_air_smoke"):
			case fnv::hash("molotov_smoking_ground_child01"):
			case fnv::hash("molotov_smoking_ground_child02"):
			case fnv::hash("molotov_smoking_ground_child02_cheapo"):
			case fnv::hash("molotov_smoking_ground_child03"):
			case fnv::hash("molotov_smoking_ground_child03_cheapo"):
			case fnv::hash("molotov_smoke_screen"):
				if (variables::visuals::world::molotov_fire) {
					for (int i = 0; i < this_ptr->active_particle; i++) {
						float* alpha = this_ptr->particle_attribute.float_attribute_pointer(PARTICLE_ATTRIBUTE_ALPHA, i);
						*alpha = variables::visuals::world::molotov_fire_clr[3];
					}
				}
				break;
			default:
				if (variables::visuals::world::molotov_fire) {
					for (int i = 0; i < this_ptr->active_particle; i++) {
						float* color = this_ptr->particle_attribute.float_attribute_pointer(PARTICLE_ATTRIBUTE_TINT_RGB, i);
						color[0] = variables::visuals::world::molotov_fire_clr[0];
						color[1] = variables::visuals::world::molotov_fire_clr[1];
						color[2] = variables::visuals::world::molotov_fire_clr[2];
					}
				}
				break;
			}
			break;
		default:
			//console::log("[particle_collection]", root_name);
			break;
	}
}