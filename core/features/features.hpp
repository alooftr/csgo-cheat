#pragma once
#include "../../dependencies/utilities/csgo.hpp"
#include "../menu/config/cheat_var.hpp"

namespace misc {
	namespace movement {
		void bunny_hop(c_usercmd* cmd);
		void edge_jump(c_usercmd* cmd);
		void edge_bug(c_usercmd* cmd);
		void jump_bug(c_usercmd* cmd);
		void infinite_crouch(c_usercmd* cmd);
	};
}

namespace visuals {

	struct box {
		int x, y, w, h;
		box() = default;
		box(int x, int y, int w, int h) {
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;
		}
	};

	bool calculate_player_box(player_t* entity, visuals::box& in);

	namespace player {
		void run();
		void animate(player_t* entity, const int index);
		void skeleton(player_t* entity, color _color, matrix_t matrix[256]);
		void name(player_t* entity, visuals::box _box);
		void box(player_t* entity, visuals::box _box);
		void flags(player_t* entity, visuals::box _box);
		void health(player_t* entity, visuals::box _box);
		void weapon(player_t* entity, visuals::box _box);
		void armor(player_t* entity, visuals::box _box);
	}
}
