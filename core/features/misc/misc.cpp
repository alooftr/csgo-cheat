#include "../features.hpp"
#include "engine_prediction.hpp"

void misc::movement::bunny_hop(c_usercmd* cmd) {
	if (!variables::movement::bunny_hop)
		return;

	if (GetAsyncKeyState(variables::movement::jump_bug_key))
		return;

	const int move_type = csgo::local_player->move_type();

	if (move_type == movetype_ladder || move_type == movetype_noclip || move_type == movetype_observer)
		return;

	if (!(csgo::local_player->flags() & fl_onground))
		cmd->buttons &= ~in_jump;
};

void misc::movement::edge_jump(c_usercmd* cmd, int old_flags) {
	if (variables::movement::edge_jump && GetAsyncKeyState(variables::movement::edge_jump_key)) {
		if (!(csgo::local_player->flags() & fl_onground) && (old_flags & fl_onground)) {
			cmd->buttons |= in_jump;
		}
	}
}

void misc::movement::edge_bug(c_usercmd* cmd, int old_flags) {
	if (variables::movement::edge_bug && GetAsyncKeyState(variables::movement::edge_bug_key)) {
		if (!(old_flags & fl_onground) && (csgo::local_player->flags() & fl_onground))
		{
			cmd->buttons &= ~in_bullrush;
			cmd->buttons |= in_duck;
		}
	}
}

void misc::movement::jump_bug(c_usercmd* cmd, int old_flags) {
	if (variables::movement::jump_bug && GetAsyncKeyState(variables::movement::jump_bug_key)) {
		if (const auto mt = csgo::local_player->move_type(); mt == movetype_ladder || mt == movetype_noclip)
			return;

		if ((prediction::prediction_flags & fl_onground) && !(csgo::local_player->flags() & fl_onground))
		{
			cmd->buttons &= ~in_bullrush;
			cmd->buttons |= in_duck;
		}

		if (csgo::local_player->flags() & fl_onground)
			cmd->buttons &= ~in_jump;
	}
}

void misc::movement::infinite_crouch(c_usercmd* cmd) {
	if (variables::movement::inifnite_crouch && csgo::local_player && csgo::local_player->is_alive())
		cmd->buttons |= in_bullrush;
}

void misc::movement::mouse_delta(c_usercmd* cmd) {
	static vec3_t ang_delta_angles;
	vec3_t ang_delta = cmd->viewangles - ang_delta_angles;
	ang_delta.clamp();

	static auto cvar_sensitivity = interfaces::console->get_convar("sensitivity");
	if (!cvar_sensitivity)
		return;

	if (ang_delta.x != 0.f) {
		static auto cvar_pitch = interfaces::console->get_convar("m_pitch");
		if (!cvar_pitch)
			return;

		int iFinalDeltaY = (int)((ang_delta.x / cvar_pitch->get_float()) / cvar_sensitivity->get_float());
		if (iFinalDeltaY <= 32767) {
			if (iFinalDeltaY >= -32768) {
				if (iFinalDeltaY >= 1 || iFinalDeltaY < 0) {
					if (iFinalDeltaY <= -1 || iFinalDeltaY > 0)
						iFinalDeltaY = iFinalDeltaY;
					else
						iFinalDeltaY = -1;
				}
				else
					iFinalDeltaY = 1;
			}
			else
				iFinalDeltaY = 32768;
		}
		else
			iFinalDeltaY = 32767;

		cmd->mousedy = (short)iFinalDeltaY;
	}

	if (ang_delta.y != 0.f) {
		static auto cvar_yaw = interfaces::console->get_convar("m_yaw");
		if (!cvar_yaw)
			return;

		int iFinalDeltaX = (int)((ang_delta.y / cvar_yaw->get_float()) / cvar_sensitivity->get_float());
		if (iFinalDeltaX <= 32767) {
			if (iFinalDeltaX >= -32768) {
				if (iFinalDeltaX >= 1 || iFinalDeltaX < 0) {
					if (iFinalDeltaX <= -1 || iFinalDeltaX > 0)
						iFinalDeltaX = iFinalDeltaX;
					else
						iFinalDeltaX = -1;
				}
				else
					iFinalDeltaX = 1;
			}
			else
				iFinalDeltaX = 32768;
		}
		else
			iFinalDeltaX = 32767;

		cmd->mousedx = (short)iFinalDeltaX;
	}

	ang_delta_angles = cmd->viewangles;
}

void misc::movement::null_strafe(c_usercmd* cmd) {
	if (!variables::movement::null_strafe || !csgo::local_player || csgo::local_player->flags() & fl_onground)
		return;

	if (cmd->mousedx > 0 && cmd->buttons & in_moveright && cmd->buttons & in_moveleft)
		cmd->sidemove = -450.f;
	else if (cmd->mousedx < 0 && cmd->buttons & in_moveleft && cmd->buttons & in_moveright)
		cmd->sidemove = 450.f;
}

void misc::movement::strafe_optimizer(c_usercmd* cmd) {
	static bool toggled = false;

	if (!variables::movement::strafe_optimizer || !csgo::local_player || csgo::local_player->flags() & fl_onground)
		return;

	static auto cvar_yaw = interfaces::console->get_convar("m_yaw");

	static auto cvar_sensitivity = interfaces::console->get_convar("sensitivity");

	static float previous_view_y = cmd->viewangles.y;
	{
		if (cmd->forwardmove == 0 && cmd->buttons & in_jump) if (auto velocity = csgo::local_player->velocity(); velocity.length_2d() > variables::movement::strafe_optimizer_min_speed)
		{
			auto client_state = interfaces::client_state;
			interfaces::prediction->update(client_state->delta_tick, client_state->delta_tick > 0,
				client_state->last_command_ack, client_state->last_outgoing_command + client_state->choked_commands);

			float mouse_yaw_factor = cvar_yaw->get_float();

			float mouse_sensitivity = cvar_sensitivity->get_float();

			float mouse_yaw_step = mouse_sensitivity * mouse_yaw_factor;

			if (cmd->sidemove < 0)
			{
				if (previous_view_y - cmd->viewangles[1] < 0)
				{
					float Strafe_Angle = remainderf(cmd->viewangles[1] - atan2f(velocity[1], velocity[0]) * 180 / M_PI, 360) * variables::movement::strafe_optimizer_desired_gain / 100;

					if (Strafe_Angle < -mouse_yaw_step)
					{
						if (Strafe_Angle < -180)
						{
							Strafe_Angle = -180;
						}

						cmd->viewangles[1] = remainderf(cmd->viewangles[1] - mouse_yaw_step * roundf(Strafe_Angle / mouse_yaw_step), 360);

						cmd->mousedx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_y - cmd->viewangles[1], 360) / sqrtf(mouse_yaw_step)));

						interfaces::engine->set_view_angles(cmd->viewangles);
					}
				}
			}
			else
			{
				if (cmd->sidemove > 0)
				{
					if (previous_view_y - cmd->viewangles[1] > 0)
					{
						float Strafe_Angle = remainderf(cmd->viewangles[1] - atan2f(velocity[1], velocity[0]) * 180 / M_PI, 360) * variables::movement::strafe_optimizer_desired_gain / 100;

						if (Strafe_Angle > mouse_yaw_step)
						{
							if (Strafe_Angle > 180)
							{
								Strafe_Angle = 180;
							}

							cmd->viewangles[1] = remainderf(cmd->viewangles[1] - mouse_yaw_step * roundf(Strafe_Angle / mouse_yaw_step), 360);

							cmd->mousedx = (__int16)(mouse_sensitivity * ceilf(remainderf(previous_view_y - cmd->viewangles[1], 360) / sqrtf(mouse_yaw_step)));

							interfaces::engine->set_view_angles(cmd->viewangles);
						}
					}
				}
			}
		}
		previous_view_y = cmd->viewangles[1];
	}
}

void misc::movement::mini_jump(c_usercmd* cmd) {
	static bool bToggled = false;
	if (!variables::movement::mini_jump && !variables::movement::mini_jump_key)
		return;

	if (csgo::local_player && csgo::local_player->is_alive()) {
		if (const auto move_type = csgo::local_player->move_type(); move_type == movetype_ladder || move_type == movetype_noclip)
			return;

		static int mini_jump_tick = 0;
		static bool mini_jump_bool = false;

		if (const auto flags = csgo::local_player->flags(); flags & fl_onground && !flags & fl_onground)
		{
			cmd->buttons |= in_jump;
			mini_jump_bool = true;
			mini_jump_tick = cmd->tick_count + 1;
		}
		if (mini_jump_bool)
		{
			if (cmd->tick_count < mini_jump_tick)
			{
				cmd->buttons |= in_duck;
				interfaces::engine->client_cmd_unrestricted("-forward");
				cmd->buttons &= ~in_back;
			}
		}
	}
}