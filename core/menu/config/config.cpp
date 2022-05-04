#include "config.hpp"
#define header "csgo_cheat config"

using namespace variables;

void config::initialize() {
	directory_path = "C:/csgo_cheat/configs";
	if (!std::filesystem::exists(directory_path))
		std::filesystem::create_directories(directory_path);
}

void save_misc(Json::Value save) {
	//Theme saving
	save[header][("menu_clr_r")][0] = variables::menu_clr[0];
	save[header][("menu_clr_g")][1] = variables::menu_clr[1];
	save[header][("menu_clr_b")][2] = variables::menu_clr[2];
	save[header][("animation_frequency")] = menu::settings::animation_frequency;
	save[header][("bg_blur")] = variables::bg_blur;
	//movement
	save[header][("bunny_hop")] = variables::movement::bunny_hop;
	save[header][("edge_jump")] = variables::movement::edge_jump;
	save[header][("edge_jump_key")] = variables::movement::edge_jump_key;
	save[header][("edge_bug")] = variables::movement::edge_bug;
	save[header][("edge_bug_key")] = variables::movement::edge_bug_key;
	save[header][("jump_bug")] = variables::movement::jump_bug;
	save[header][("jump_bug_key")] = variables::movement::jump_bug_key;

	save[header][("mini_jump")] = variables::movement::mini_jump;
	save[header][("mini_jump_key")] = variables::movement::mini_jump_key;

	save[header][("infinite_crouch")] = variables::movement::inifnite_crouch;
	save[header][("null_strafe")] = variables::movement::null_strafe;

	save[header][("strafe_optimizer")] = variables::movement::strafe_optimizer;
	save[header][("strafe_optimizer_min_speed")] = variables::movement::strafe_optimizer_min_speed;
	save[header][("strafe_optimizer_desired_gain")] = variables::movement::strafe_optimizer_desired_gain;

	save[header][("mini_jump")] = variables::movement::mini_jump;
	save[header][("mini_jump_key")] = variables::movement::mini_jump_key;

	//misc
	save[header][("auto_pistol")] = variables::misc::auto_pistol;
}

void load_misc(Json::Value load) {
	//theme save
	variables::menu_clr[0] = load[header][("menu_clr_r")][0].asFloat();
	variables::menu_clr[1] = load[header][("menu_clr_g")][1].asFloat();
	variables::menu_clr[2] = load[header][("menu_clr_b")][2].asFloat();
	menu::settings::animation_frequency = load[header][("animation_frequency")].asFloat();
	variables::bg_blur = load[header][("bg_blur")].asBool();
	//movement
	variables::movement::bunny_hop = load[header][("bunny_hop")].asBool();

	variables::movement::edge_jump = load[header][("edge_jump")].asBool();
	variables::movement::edge_jump_key = load[header][("edge_jump_key")].asInt();
	variables::movement::edge_bug = load[header][("edge_bug")].asBool();
	variables::movement::edge_bug_key = load[header][("edge_bug_key")].asInt();
	variables::movement::jump_bug = load[header][("jump_bug")].asBool();
	variables::movement::jump_bug_key = load[header][("jump_bug_key")].asInt();


	variables::movement::mini_jump = load[header][("mini_jump")].asBool();
	variables::movement::mini_jump_key = load[header][("mini_jump_key")].asInt();

	variables::movement::inifnite_crouch = load[header][("infinite_crouch")].asBool();
	variables::movement::null_strafe = load[header][("null_strafe")].asBool();

	variables::movement::strafe_optimizer = load[header][("strafe_optimizer")].asBool();
	variables::movement::strafe_optimizer_min_speed = load[header][("strafe_optimizer_min_speed")].asInt();
	variables::movement::strafe_optimizer_desired_gain = load[header][("strafe_optimizer_desired_gain")].asInt();

	variables::movement::mini_jump = load[header][("mini_jump")].asBool();
	variables::movement::mini_jump_key = load[header][("mini_jump_key")].asInt();

	//misc
	variables::misc::auto_pistol = load[header][("auto_pistol")].asBool();

}

void save_visuals(Json::Value save) {
	//world
	save[header][("molotov_fire")] = variables::visuals::world::molotov_fire;

	save[header][("molotov_fire")][0] = variables::visuals::world::molotov_fire_clr[0];
	save[header][("molotov_fire")][1] = variables::visuals::world::molotov_fire_clr[1];
	save[header][("molotov_fire")][2] = variables::visuals::world::molotov_fire_clr[2];
	save[header][("molotov_fire")][3] = variables::visuals::world::molotov_fire_clr[3];
}

void load_visuals(Json::Value load) {
	//world
	variables::visuals::world::molotov_fire = load[header][("molotov_fire")].asBool();

	variables::visuals::world::molotov_fire_clr[0] = load[header][("molotov_fire")][0].asFloat();
	variables::visuals::world::molotov_fire_clr[1] = load[header][("molotov_fire")][1].asFloat();
	variables::visuals::world::molotov_fire_clr[2] = load[header][("molotov_fire")][2].asFloat();
	variables::visuals::world::molotov_fire_clr[3] = load[header][("molotov_fire")][3].asFloat();
}

void config::save(const std::string name) {
	const auto path = directory_path + "/" + name + ".json";
	std::ofstream out(path);

	if (!out.is_open())
		return;

	Json::Value save;

	save_misc(save);
	save_visuals(save);

	out << save;
	out.close();
}

void config::load(const std::string name) {
	const auto path = directory_path + "/" + name + ".json";
	std::ifstream in(path);

	if (!in.good())
		save(name);

	if (!in.is_open())
		return;

	Json::Value load;

	in >> load;

	load_misc(load);
	load_visuals(load);

	in.close();
}