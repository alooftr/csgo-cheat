#include "../features.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::movement::bunny_hop)
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::movement::edge_jump(c_usercmd* cmd, int backup_flags) {
	if (variables::movement::edge_jump && GetAsyncKeyState(variables::movement::edge_jump_key)) {
		if (!(csgo::local_player->flags() & fl_onground) && (backup_flags & fl_onground)) {
			console::log("[info]", "Holding edge_jump_key and trying to jump");
			cmd->buttons |= in_jump;
		}
	}
}