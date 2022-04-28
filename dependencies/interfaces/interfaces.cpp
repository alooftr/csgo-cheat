#include "interfaces.hpp"
#include "../utilities/csgo.hpp"

template <typename T>
static constexpr auto relativeToAbsolute(uintptr_t address) noexcept
{
	return (T)(address + 4 + *reinterpret_cast<std::int32_t*>(address));
}

static std::pair<void*, std::size_t> getModuleInformation(const char* name) noexcept
{
	if (HMODULE handle = GetModuleHandleA(name)) {
		if (MODULEINFO moduleInfo; GetModuleInformation(GetCurrentProcess(), handle, &moduleInfo, sizeof(moduleInfo)))
			return std::make_pair(moduleInfo.lpBaseOfDll, moduleInfo.SizeOfImage);
	}
	return {};
}

[[nodiscard]] static auto generateBadCharTable(std::string_view pattern) noexcept
{
	assert(!pattern.empty());

	std::array<std::size_t, (std::numeric_limits<std::uint8_t>::max)() + 1> table;

	auto lastWildcard = pattern.rfind('?');
	if (lastWildcard == std::string_view::npos)
		lastWildcard = 0;

	const auto defaultShift = (std::max)(std::size_t(1), pattern.length() - 1 - lastWildcard);
	table.fill(defaultShift);

	for (auto i = lastWildcard; i < pattern.length() - 1; ++i)
		table[static_cast<std::uint8_t>(pattern[i])] = pattern.length() - 1 - i;

	return table;
}

static std::uintptr_t findPattern(const char* moduleName, std::string_view pattern, bool reportNotFound = true) noexcept
{
	static auto id = 0;
	++id;

	const auto [moduleBase, moduleSize] = getModuleInformation(moduleName);

	if (moduleBase && moduleSize) {
		const auto lastIdx = pattern.length() - 1;
		const auto badCharTable = generateBadCharTable(pattern);

		auto start = static_cast<const char*>(moduleBase);
		const auto end = start + moduleSize - pattern.length();

		while (start <= end) {
			int i = lastIdx;
			while (i >= 0 && (pattern[i] == '?' || start[i] == pattern[i]))
				--i;

			if (i < 0)
				return reinterpret_cast<std::uintptr_t>(start);

			start += badCharTable[static_cast<std::uint8_t>(start[lastIdx])];
		}
	}

	if (reportNotFound)
		MessageBoxA(NULL, ("Failed to find pattern #" + std::to_string(id) + '!').c_str(), "Osiris", MB_OK | MB_ICONWARNING);
	return 0;
}

bool interfaces::initialize() {
	client = get_interface<i_base_client_dll, interface_type::index>("client.dll", "VClient018");
	if (client)
		console::log("[interfaces]", "client found at %p", client);

	entity_list = get_interface<i_client_entity_list, interface_type::index>("client.dll", "VClientEntityList003");
	if (entity_list)
		console::log("[interfaces]", "entity_list found at %p", entity_list);

	engine = get_interface<iv_engine_client, interface_type::index>("engine.dll", "VEngineClient014");
	if (engine)
		console::log("[interfaces]", "engine found at %p", engine);

	panel = get_interface<i_panel, interface_type::index>("vgui2.dll", "VGUI_Panel009");
	if (panel)
		console::log("[interfaces]", "panel found at % p", panel);

	surface = get_interface<i_surface, interface_type::index>("vguimatsurface.dll", "VGUI_Surface031");
	if (surface)
		console::log("[interfaces]", "surface found at % p", surface);

	material_system = get_interface<i_material_system, interface_type::index>("materialsystem.dll", "VMaterialSystem080");
	if (material_system)
		console::log("[interfaces]","material_system found at %p", material_system);

	model_info = get_interface<iv_model_info, interface_type::index>("engine.dll", "VModelInfoClient004");
	if (model_info)
		console::log("[interfaces]" ,"model_info found at %p", model_info);

	model_render = get_interface<iv_model_render, interface_type::index>("engine.dll", "VEngineModel016");
	if (model_render)
		console::log("[interfaces]", "model_render found at %p", model_render);

	render_view = get_interface<i_render_view, interface_type::index>("engine.dll", "VEngineRenderView014");
	if (render_view)
		console::log("[interfaces]", "render_view found at %p", render_view);

	console = get_interface<i_console, interface_type::index>("vstdlib.dll", "VEngineCvar007");
	if (console)
		console::log("[interfaces]" ,"console found at %p", console);

	localize = get_interface<i_localize, interface_type::index>("localize.dll", "Localize_001");
	if (localize)
		console::log("[interfaces]" ,"localize found at %p", localize);

	event_manager = get_interface<i_game_event_manager2, interface_type::index>("engine.dll", "GAMEEVENTSMANAGER002");
	if (event_manager)
		console::log("[interfaces]" ,"event_manager found at %p", event_manager);

	debug_overlay = get_interface<iv_debug_overlay, interface_type::index>("engine.dll", "VDebugOverlay004");
	if (debug_overlay)
		console::log("[interfaces]" ,"debug_overlay found at %p", debug_overlay);

	inputsystem = get_interface<i_inputsytem, interface_type::index>("inputsystem.dll", "InputSystemVersion001");
	if (inputsystem)
		console::log("[interfaces]" ,"inputsystem found at %p", inputsystem);

	trace_ray = get_interface<trace, interface_type::index>("engine.dll", "EngineTraceClient004");
	if (trace_ray)
		console::log("[interfaces]" ,"trace_ray found at %p", trace_ray);

	game_movement = get_interface<player_game_movement, interface_type::index>("client.dll", "GameMovement001");
	if (game_movement)
		console::log("[interfaces]" ,"game_movement found at %p", game_movement);

	prediction = get_interface<player_prediction, interface_type::index>("client.dll", "VClientPrediction001");
	if (prediction)
		console::log("[interfaces]" ,"prediction found at %p", prediction);

	file_system = get_interface<i_filesystem, interface_type::index>("filesystem_stdio.dll", "VFileSystem017");
	if (file_system)
		console::log("[interfaces]" ,"entity_list found at %p", file_system);

	// get the exported KeyValuesSystem function
	if (const HINSTANCE handle = GetModuleHandle("vstdlib.dll"))
	{		// set our pointer by calling the function
		key_values_system = reinterpret_cast<void* (__cdecl*)()>(GetProcAddress(handle, "KeyValuesSystem"))();
		if (key_values_system)
			console::log("[interfaces]" ,"key_values_system found at %p", key_values_system);
	}
	/*custom interfaces*/
	client_mode = **reinterpret_cast<i_client_mode * **>((*reinterpret_cast<uintptr_t * *>(client))[10] + 5);
	if (client_mode)
		console::log("[custom interfaces]" ,"client_mode found at %p", client_mode);

	globals = **reinterpret_cast<c_global_vars_base***>((*reinterpret_cast<uintptr_t**>(client))[11] + 10);
	if (globals)
		console::log("[custom interfaces]" ,"globals found at %p", globals);

	client_state = **reinterpret_cast<i_client_state***>(utilities::pattern_scan("engine.dll", sig_client_state) + 1);
	if (client_state)
		console::log("[custom interfaces]" ,"client_state found at %p", client_state);

	directx = **reinterpret_cast<IDirect3DDevice9***>(utilities::pattern_scan("shaderapidx9.dll", sig_directx) + 1);
	if (directx)
		console::log("[custom interfaces]" ,"directx found at %p", directx);

	input = *reinterpret_cast<i_input**>(utilities::pattern_scan("client.dll", sig_input) + 1);
	if (input)
		console::log("[custom interfaces]" ,"input found at %p", input);

	glow_manager = reinterpret_cast<glow_manager_t*>(*reinterpret_cast<uintptr_t*>(utilities::pattern_scan("client.dll", sig_glow_manager) + 3));
	if (glow_manager)
		console::log("[custom interfaces]" ,"glow_manager found at %p", glow_manager);

	move_helper = **reinterpret_cast<player_move_helper***>(utilities::pattern_scan("client.dll", sig_player_move_helper) + 2);
	if (move_helper)
		console::log("[custom interfaces]" ,"move_helper found at %p", move_helper);

	weapon_system = *reinterpret_cast<i_weapon_system**>(utilities::pattern_scan("client.dll", sig_weapon_data) + 2);
	if (weapon_system)
		console::log("[custom interfaces]" ,"weapon_system found at %p", weapon_system);

	device = **(IDirect3DDevice9***)(utilities::pattern_scan("shaderapidx9.dll", sig_device) + 1);
	if (device)
		console::log("[custom interfaces]", ("IDirect3DDevice9 found at 0x % p "), device);

	//memory stuffs
	alloc_key_values_client = utilities::pattern_scan("client.dll", sig_alloc_key_value) + 3;
	alloc_key_values_engine = utilities::pattern_scan("engine.dll", sig_alloc_key_value) + 3;
	is_depth_of_field_enabled = utilities::pattern_scan("client.dll", sig_dop_enabled);
	//c_particle_collection_simulate = reinterpret_cast<CParticleCollection*>(utilities::pattern_scan("client.dll", "55 8B EC 83 E4 F8 83 EC 30 56 57 8B F9 0F") + 1);
	//pasted this function lmfao sorry
	draw_screen_effect_material = relativeToAbsolute<uintptr_t>(findPattern("client.dll", "\xE8????\x83\xC4\x0C\x8D\x4D\xF8") + 1);

	console::log("[interfaces]" ,"initialized!");

	return true;
}
