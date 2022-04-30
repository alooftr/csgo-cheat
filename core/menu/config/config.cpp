#include "config.hpp"
#define config_header "csgo_cheat config"

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
	save[config_header][("menu_clr_r")][0] = variables::menu_clr[0];
	save[config_header][("menu_clr_g")][1] = variables::menu_clr[1];
	save[config_header][("menu_clr_b")][2] = variables::menu_clr[2];

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

	variables::menu_clr[0] = load[config_header][("menu_clr_r")][0].asFloat();
	variables::menu_clr[1] = load[config_header][("menu_clr_g")][1].asFloat();
	variables::menu_clr[2] = load[config_header][("menu_clr_b")][2].asFloat();


	//test

	in.close();
}

void config::del(std::string name) {
	const auto path = directory_path + "/" + name;

	if (!std::filesystem::exists(path))
		return;

	std::filesystem::remove(path.c_str());
}