#pragma once
#include <windows.h>
#include <iostream>
#include <cstdint>
#include <memory>
#include <vector>
#include <thread>
#include <chrono>
#include <array>
#include <fstream>
#include <istream>
#include <unordered_map>
#include <intrin.h>
#include <filesystem>
#include <stdio.h>
#include <time.h>
#include "../resources/xorstr.hpp"

#include "../utilities/singleton.hpp"
#include "../utilities/fnv.hpp"
#include "../utilities/utilities.hpp"
#include "../../dependencies/minhook/minhook.h"
#include "../interfaces/interfaces.hpp"
#include "../../source-sdk/sdk.hpp"
#include "../../core/hooks/hooks.hpp"
#include "../../dependencies/math/math.hpp"
#include "../../dependencies/utilities/renderer/renderer.hpp"
#include "../../dependencies/utilities/console/console.hpp"
#include "../utilities/csgo.hpp"

#include "../../dependencies/imgui/imgui.h"
#include "../../dependencies/imgui/imgui_impl_dx9.h"
#include "../../dependencies/imgui/imgui_impl_win32.h"

//interfaces
#define sig_client_state xor("A1 ? ? ? ? 8B 80 ? ? ? ? C3")
#define sig_directx xor("A1 ? ? ? ? 50 8B 08 FF 51 0C")
#define sig_input xor("B9 ? ? ? ? F3 0F 11 04 24 FF 50 10")
#define sig_glow_manager xor("0F 11 05 ? ? ? ? 83 C8 01 C7 05 ? ? ? ? 00 00 00 00")
#define sig_player_move_helper xor("8B 0D ? ? ? ? 8B 46 08 68")
#define sig_weapon_data xor("8B 35 ? ? ? ? FF 10 0F B7 C0")
#define sig_alloc_key_value xor("FF 52 04 85 C0 74 0C 56")
#define sig_device xor("A1 ? ? ? ? 50 8B 08 FF 51 0C")
#define sig_dop_enabled xor("8B ? ? ? ? ? 56 8B 01 FF 50 34 8B F0 85 F6 75")
//misc
#define sig_set_angles xor("55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1")
#define sig_prediction_random_seed xor("8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04")
#define sig_file_system xor("8B 0D ? ? ? ? 8D 95 ? ? ? ? 6A 00 C6")
#define sig_cam_think xor("85 C0 75 30 38 86")
#define sig_list_leaves xor("56 52 FF 50 18")
#define sig_color_modulation xor("55 8B EC 83 EC ? 56 8B F1 8A 46")
#define sig_is_using_static_prop_debug_modes xor("8B 0D ? ? ? ? 81 F9 ? ? ? ? 75 ? A1 ? ? ? ? 35 ? ? ? ? EB ? 8B 01 FF 50 ? 83 F8 ? 0F 85 ? ? ? ? 8B 0D")
#define sig_check_file_CRCs_with_server xor("55 8B EC 81 EC ? ? ? ? 53 8B D9 89 5D F8 80")
#define sig_particle_collection_simulate xor("55 8B EC 83 E4 F8 83 EC 30 56 57 8B F9 0F")

namespace csgo {
	extern player_t* local_player;
}