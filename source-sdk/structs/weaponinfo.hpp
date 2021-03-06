#pragma once

class weapon_info_t {
public:
	char	pad0[4];
	char* console_name;
	char	pad1[12];
	int		max_clip1;
	int		max_clip2;
	int		default_clip1;
	int		default_clip2;
	int		primary_reserve_ammo_max;
	int		secondary_reserve_ammo_max;
	char* world_model;
	char* view_model;
	char* dropped_model;
	char	pad2[80];
	char* hud_name;
	char* weapon_name;
	char	pad3[2];
	bool	is_melee_weapon;
	char	pad4[9];
	float	weapon_weight;
	char	pad5[40];
	int		type;
	char	pad6[4];
	int		weapon_price;
	int		kill_award;
	char	pad7[4];
	float	cycle_time;
	float	cycle_time_alt;
	char	pad8[8];
	bool	full_auto;
	char	pad9[3];
	int		damage;
	float	armor_ratio;
	int		bullets;
	float	penetration;
	char	pad10[8];
	float	range;
	float	range_modifier;
	float	throw_velocity;
	char	pad11[12];
	bool	has_silencer;
	char	pad12[11];
	char* bullet_type;
	float	max_speed;
	float	max_speed_alt;
	char	pad13[4];
	float	spread;
	float	spread_alt;
	float	inaccuracy_crouch;
	float	inaccuracy_crouch_alt;
	float	inaccuracy_stand;
	float	inaccuracy_stand_alt;
	float	inaccuracy_jump_initial;
	float	inaccuracy_jump_apex;
	float	inaccuracy_jump;
	float	inaccuracy_jump_alt;
	float	inaccuracy_land;
	float	inaccuracy_land_alt;
	float	inaccuracy_ladder;
	float	inaccuracy_ladder_alt;
	float	inaccuracy_fire;
	float	inaccuracy_fire_alt;
	float	inaccuracy_move;
	float	inaccuracy_move_alt;
	float	inaccuracy_reload;
	int		recoil_seed;
	float	recoil_angle;
	float	recoil_angle_alt;
	float	recoil_angle_variance;
	float	recoil_angle_variance_alt;
	float	recoil_magnitude;
	float	recoil_magnitude_alt;
	float	recoil_magnitude_variance;
	float	recoil_magnitude_variance_alt;
};
