#include "framework.hpp"
#include <algorithm>
//credits to harcuz for menu framework (https://www.unknowncheats.me/forum/members/2669363.html),
POINT cursor;
POINT cursor_corrected;

bool mouse_in_range(int x, int y, int x2, int y2)
{
	if (cursor.x > x && cursor.y > y && cursor.x < x2 + x && cursor.y < y2 + y)
		return true;

	return false;
}

void menu_framework::group_box(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, bool show_label) {
	//groupbox background
	render::draw_filled_rect(x, y, w, h, color(25, 25, 25, 255));

	//groupbox outline
	render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

	//groupbox label
	if (show_label)
		render::text(x + 2, y - 12, font, string, false, color::white());
}

void menu_framework::tab(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h, unsigned long font, const std::string string, std::int32_t& tab, std::int32_t count, bool show_outline) {
	GetCursorPos(&cursor);

	if ((cursor.x > x) && (cursor.x < x + w) && (cursor.y > y) && (cursor.y < y + h) && (GetAsyncKeyState(VK_LBUTTON) & 1)) {
		//render::draw_outline(position + 2, y - 1, w, h, color(72, 72, 72, 255));
		tab = count;
	}

	//tab background
	if (show_outline) {
		render::draw_filled_rect(x, y, w, h, color(32, 32, 32, 255));
		render::draw_filled_rect(x, y, w, 2, color(25, 25, 25, 255));
		render::draw_rect(x, y, w, h, color(45, 45, 45, 255));

		//render::draw_rect(x, y, w, h, tab == count ? color::white() : color(32, 32, 32, 255));
	}
	//tab label
	render::text(x - render::get_text_size(font, string).x / 2 + 50, y + h / 2 - 8, font, string, false, tab == count ? color::menu() : color::white());
}

void menu_framework::check_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, bool& value) {
	GetCursorPos(&cursor);

	int w = 15, h = 10;

	//checkbox label
	render::text(x + 2, y - 1, font, string, false, color::white());

	//checkbox background
	render::draw_filled_rect(position - 5, y, w, h, value ? color::menu() : color(36, 36, 36, 255));

	if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y - 2) && (cursor.y < y + h)) {
		render::draw_outline(position - 5, y, w, h, color(72, 72, 72, 255));

		if (GetAsyncKeyState(VK_LBUTTON) & 1)
			value = !value;
	}
}

void menu_framework::slider_float(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, float& value, float min_value, float max_value) {
	GetCursorPos(&cursor);

	int ix = x + 140;
	int yi = y + 4;

	//slider label  (std::stringstream{ } << string << ": " << std::setprecision(3) << value).str()
	render::text(x + 2, y - 1, font, string, false, color::white());

	//slider background
	render::draw_filled_rect(ix, yi, position, 6, color(36, 36, 36, 255));
	render::draw_filled_rect(ix, yi, (value - min_value) / (max_value - min_value) * position, 6, color::menu());


	if ((cursor.x > ix - 10) && (cursor.x < ix + position + 10) && (cursor.y > yi - 10 ) && (cursor.y < yi + 10)) {
		//render::draw_outline(ix, yi - 6, position, 16, color(72, 72, 72, 255));
		//render::draw_filled_rect(ix, yi - 6, position, 16, color(36, 36, 36, 255));
		//render::draw_filled_rect(ix, yi - 6, (value - min_value) / (max_value - min_value) * position, 16, color::menu());
		render::text(ix + (value - min_value) / (max_value - min_value) * position - 20, y - 1, font, (std::stringstream{ } << std::setprecision(3) << value).str(), false, color::white());

		if (GetAsyncKeyState(VK_LBUTTON))
			//value = (cursor.x - ix) / (float(position) / float(max_value));
			value = std::clamp(float(min_value + (max_value - min_value) * (cursor.x - ix) / position), float(min_value), float(max_value));
	}
}

void menu_framework::slider_int(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, int& value, int min_value, int max_value) {
	GetCursorPos(&cursor);

	int ix = x + 140;
	int yi = y + 4;

	//slider label
	render::text(x + 2, y - 1, font, string, false, color::white());

	//slider background
	render::draw_filled_rect(ix, yi, position, 6, color(36, 36, 36, 255));
	render::draw_filled_rect(ix, yi, (value - min_value) / (max_value - min_value) * position, 6, color::menu());

	if ((cursor.x > ix - 10) && (cursor.x < ix + position + 10) && (cursor.y > yi - 10) && (cursor.y < yi + 10)) {
		//render::draw_outline(ix, yi - 6, position, 16, color(72, 72, 72, 255));
		//render::draw_filled_rect(ix, yi - 6, position, 16, color(36, 36, 36, 255));
		//render::draw_filled_rect(ix, yi - 6, (value - min_value) / (max_value - min_value) * position, 16, color::menu());
		render::text(ix + (value - min_value) / (max_value - min_value) * position - 20, y - 1, font, std::to_string(value), false, color::white());

		if (GetAsyncKeyState(VK_LBUTTON))
			//value = (cursor.x - ix) / (int(position) / int(max_value));
			value = std::clamp(int(min_value + (max_value - min_value) * (cursor.x - ix) / position), int(min_value), int(max_value));

	}
	//+ button
}


void menu_framework::combo_box(std::int32_t x, std::int32_t y, std::int32_t position, unsigned long font, const std::string string, const std::vector<std::string> elements, int* values) {
	GetCursorPos(&cursor);

	int w = 125, h = 15;
	std::size_t size = elements.size();

	bool open = false;
	static bool selected_opened = false;
	static bool rest;
	static std::string name_selected;

	if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h)) {
		render::draw_outline(position + 2, y - 1, w, h, color(72, 72, 72, 255));
		if (GetAsyncKeyState(VK_LBUTTON) & 1) {
			name_selected = string;
			if (!rest)
				selected_opened = !selected_opened;
			rest = true;
		}
	}
	else
		rest = false;
	if (name_selected == string)
		open = selected_opened;

	if (open) {
		//render::draw_filled_rect(position + 2, y - 1, w, 15 + (size * 15), color(36, 36, 36, 255));
		for (int i = 0; i < size; i++) {
			if ((cursor.x > position) && (cursor.x < position + w) && (cursor.y > y) && (cursor.y < y + h + 15 + (i * 15)) && GetAsyncKeyState(VK_LBUTTON) & 1) {
				*values = i;
				//In order for the combobox to close when the user selects an option.
				selected_opened = !selected_opened;
				return;
				//In order for the combobox to close when the user selects an option.
			}
			render::draw_outline(position + 2, y - 1, w + position, h, color(72, 72, 72, 255));
			render::text(position + 7, (y - 1) + 15 + (i * 15), font, elements[i].c_str(), false, i == *values ? color::white() : color::white(100));
		}
	}

	render::text(x + 2, y - 1, font, string, false, color::white());
	render::draw_filled_rect(position + 2, y - 1, w, h, color(36, 36, 36, 255));
	render::text(position + 7, y - 1, font, elements[*values].c_str(), false, color::white());
}

void menu_framework::color_picker(std::int32_t x, std::int32_t y, std::int32_t position, std::string name, color& output_value, bool& is_opened) {
	GetCursorPos(&cursor);
	auto in_range = mouse_in_range(position - 4, y, 15, 10);
	//static bool is_opened = false;

	render::text(x, y, render::fonts::menu_font, name, false, color::white());

	render::draw_filled_rect(position - 4, y, 15, 10, color(output_value.r, output_value.g, output_value.b));
	if (in_range && GetAsyncKeyState(VK_LBUTTON) & 1) {
		is_opened = true;
	}
	else if (in_range && GetAsyncKeyState(VK_RBUTTON) & 1) {
		//randomize color cuz why not?
		output_value = color::rainbow(interfaces::globals->realtime, 4);
		return;
	}

	if (in_range)
		render::draw_outline(position - 4, y, 15, 10, color(72, 72, 72, 255));

	if (is_opened) {
		static float h = 1.f, s = 1.f, v = 1.f, a = 1.f;
		static float v_2 = 1.f;


		int cp_x = position + 10;
		int cp_render_x = position + 15;
		int cp_y = y;
		int cp_render_y = y + 5;
		int cp_w = 150;
		int cp_main_w = 175;
		int cp_main_h = 200;
		int cp_h = 150;

		int button_y = cp_render_y + 150 + 20;
		int hsv_x = cp_render_x + cp_w + 5;

		if (!mouse_in_range(cp_x, cp_y, cp_main_w, cp_main_h) && GetAsyncKeyState(VK_LBUTTON) & 1)
			is_opened = false;

		static float full_sat_r, full_sat_g, full_sat_b;
		//main picker
		render::convert_hsv_to_rgb(h, 1.f, 1.f, full_sat_r, full_sat_g, full_sat_b);
		render::draw_filled_rect(cp_x, cp_y, cp_main_w, cp_main_h, color(25, 25, 25));
		render::draw_outline(cp_x, cp_y, cp_main_w, cp_main_h, color(72, 72, 72, 255));
		render::draw_fade(cp_render_x, cp_render_y, cp_w, cp_h, color(255, 255, 255), color(full_sat_r * 255, full_sat_g * 255, full_sat_b * 255), true);
		render::draw_fade(cp_render_x, cp_render_y + (cp_h / 2), cp_w, cp_h / 2, color(0, 0, 0, 0), color(0, 0, 0), false);

		//rainbow slider	
		const color col_hues[6 + 1] = { color(255,0,0,255), color(255,255,0,255), color(0,255,0,255), color(0,255,255,255), color(0,0,255,255), color(255,0,255,255), color(255,0,0,255) };
		for (int i = 0; i < 6; i++)
			render::draw_fade(hsv_x, cp_render_y + ((150 / 6) * i), 10, 150 / 6, col_hues[i], col_hues[i + 1], false);

		render::draw_fade(cp_render_x, cp_render_y + cp_h + 5, 150, 10, color(0, 0, 0), color::white(), true);

		//main math
		auto main_box_in_range = mouse_in_range(cp_render_x, cp_render_y, 150, 150);
		auto hsv_box_in_range = mouse_in_range(hsv_x, cp_render_y, 10, 150);
		auto alpha_in_range = mouse_in_range(cp_render_x, cp_render_y + cp_h + 5, 150, 10);

		render::draw_rect(cp_render_x, cp_render_y, 150, 150, main_box_in_range ? color(255, 255, 255) : color(50, 50, 50));
		render::draw_rect(hsv_x, cp_render_y, 10, 150, hsv_box_in_range ? color(255, 255, 255) : color(50, 50, 50));
		render::draw_rect(cp_render_x, cp_render_y + cp_h + 5, 150, 10, alpha_in_range ? color(255, 255, 255) : color(50, 50, 50));

		static float clr_r = output_value.r / 255,
					 clr_g = output_value.g / 255,
					 clr_b = output_value.b / 255;
		render::convert_hsv_to_rgb(h, s, v, clr_r, clr_g, clr_b);

		if (main_box_in_range && GetAsyncKeyState(VK_LBUTTON)) {
			s = (cursor.x - cp_render_x) / (float(150) / float(1));
			v = 1 - (cursor.y - cp_render_y) / (float(150) / float(1));
			v_2 = (cursor.y - cp_render_y) / (float(150) / float(1));

			output_value.r = clr_r * 255;
			output_value.g = clr_g * 255;
			output_value.b = clr_b * 255;
		}
		if (hsv_box_in_range && GetAsyncKeyState(VK_LBUTTON)) {
			h = (cursor.y - cp_render_y) / (float(150) / float(1));
		}
		if (alpha_in_range && GetAsyncKeyState(VK_LBUTTON)) {
			a = (cursor.x - cp_render_x) / (float(150) / float(1));
			output_value.a = a * 255;
		}

		//indicator thingys
		render::draw_rect(cp_render_x + (s * 150) - 2, cp_render_y + (v_2 * 150) - 2, 6, 6, color(0, 0, 0));
		render::draw_rect(hsv_x, cp_render_y + (h * 150), 10, 5, color(0, 0, 0));
		render::draw_rect(cp_render_x + (a * 150) - 2, cp_render_y + cp_h + 5, 5, 10, color(0, 0, 0));

		
		render::draw_filled_rect(cp_render_x, button_y, cp_main_w - 7.5, 20, color(20, 20, 20));
		render::draw_outline(cp_render_x, button_y, cp_main_w - 7.5, 20, color(72, 72, 72, 255));
		std::string str = std::format("r: {}, g: {}, b: {}, a: {}", output_value.r, output_value.g, output_value.b, output_value.a);
		render::text(cp_render_x + (cp_main_w / 2 - 7.5), button_y + 3, render::fonts::menu_font, str, true, color(255, 255, 255));
	}
}

void menu_framework::menu_movement(std::int32_t& x, std::int32_t& y, std::int32_t w, std::int32_t h) {
	GetCursorPos(&cursor);

	if (GetAsyncKeyState(VK_LBUTTON) < 0 && (cursor.x > x && cursor.x < x + w && cursor.y > y && cursor.y < y + h)) {
		should_drag = true;

		if (!should_move) {
			cursor_corrected.x = cursor.x - x;
			cursor_corrected.y = cursor.y - y;
			should_move = true;
		}
	}

	if (should_drag) {
		x = cursor.x - cursor_corrected.x;
		y = cursor.y - cursor_corrected.y;
	}

	if (GetAsyncKeyState(VK_LBUTTON) == 0) {
		should_drag = false;
		should_move = false;
	}
}

#ifdef _DEBUG
void menu_framework::debug() {
	GetCursorPos(&cursor);
	
	std::string text = std::format("cursor.x: {}, cursor.y: {}", cursor.x, cursor.y);
	vec2_t size = render::get_text_size(render::fonts::menu_font, text);
	//1 pixel on left/right and 2 on top/bottom
	render::draw_filled_rect(cursor.x + 2, cursor.y + 20, size.x + 5, size.y, color(32, 32, 32, 165));
	render::draw_line(cursor.x + 2, cursor.y + 20, cursor.x + size.x + 7, cursor.y + 20, color::rainbow(interfaces::globals->realtime));

	render::text(cursor.x + 5, cursor.y + 20, render::fonts::menu_font, text, false, color::white());
}
#endif