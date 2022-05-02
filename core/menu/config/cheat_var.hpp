#pragma once
#include "../../../source-sdk/misc/color.hpp"

namespace variables {
	inline float copy_paste_color[4]{ 1.f , 1.f, 1.f, 1.f };
	inline float menu_clr[3]{ 85.f / 255 , 125.f / 255, 200.f / 255 };

	namespace movement {
		inline bool bunny_hop = false;

		inline bool edge_jump = false;
		inline int edge_jump_key = 0;

		inline bool edge_bug = false;
		inline int edge_bug_key = 0;

		inline bool jump_bug = false;
		inline int jump_bug_key = false;

		inline bool inifnite_crouch = false;

		inline bool null_strafe = false;

		inline bool strafe_optimizer = false;
		inline int strafe_optimizer_min_speed = 250;
		inline int strafe_optimizer_desired_gain = 50;

		inline bool mini_jump = false;
		inline int mini_jump_key = 0;



	}
}
