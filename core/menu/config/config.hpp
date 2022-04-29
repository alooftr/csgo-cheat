#pragma once
#include <filesystem>
#include <fstream>

#include "../../../dependencies/utilities/json/json.hpp"
#include "../config/cheat_var.hpp"

namespace config {
	void initialize();
	void load(const std::string name);
	void save(const std::string name);

	inline std::string directory_path;
}