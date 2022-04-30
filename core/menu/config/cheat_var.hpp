#pragma once
#include "../../../source-sdk/misc/color.hpp"

namespace variables {
	inline float copy_paste_color[4]{ 1.f , 1.f, 1.f, 1.f };
	inline float menu_clr[3]{ 85.f / 255 , 125.f / 255, 200.f / 255 };
	

	namespace movement {
		inline bool bunny_hop = false;
		inline bool edge_jump = false;
		inline int edge_jump_key = 0;
	}
}
