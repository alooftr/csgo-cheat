#include "config.hpp"
#define header "csgo_cheat config"

using namespace variables;

void config::initialize() {
	directory_path = "C:/csgo_cheat/configs";
	if (!std::filesystem::exists(directory_path))
		std::filesystem::create_directories(directory_path);
}

void config::save(const std::string name) {
	const auto path = directory_path + "/" + name + ".json";
	std::ofstream out(path);

	if (!out.is_open())
		return;

	Json::Value save;
	//Theme saving
	save[header][("menu_clr_r")][0] = variables::menu_clr[0];
	save[header][("menu_clr_g")][1] = variables::menu_clr[1];
	save[header][("menu_clr_b")][2] = variables::menu_clr[2];
	save[header][("animation_frequency")] = menu::settings::animation_frequency;

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

	variables::menu_clr[0] = load[header][("menu_clr_r")][0].asFloat();
	variables::menu_clr[1] = load[header][("menu_clr_g")][1].asFloat();
	variables::menu_clr[2] = load[header][("menu_clr_b")][2].asFloat();

	menu::settings::animation_frequency   = load[header][("animation_frequency")].asFloat();

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

	//test

	in.close();
}

void config::del(std::string name) {
	const auto path = directory_path + "/" + name;

	if (!std::filesystem::exists(path))
		return;

	std::filesystem::remove(path.c_str());
}