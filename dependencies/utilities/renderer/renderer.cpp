#include "renderer.hpp"

unsigned long render::fonts::menu_font;

void render::initialize() {
	render::fonts::menu_font = interfaces::surface->font_create();

	interfaces::surface->set_font_glyph(render::fonts::menu_font, "Tahoma", 12, 500, 0, 0, font_flags::fontflag_outline);

	console::log("[info]", "render initialized!");
}

void render::draw_line(std::int32_t x1, std::int32_t y1, std::int32_t x2, std::int32_t y2, color colour) {
	interfaces::surface->set_drawing_color(colour.r, colour.g, colour.b, colour.a);
	interfaces::surface->draw_line(x1, y1, x2, y2);
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, const wchar_t* text, bool centered, color color) {
	interfaces::surface->draw_text_font(font);
	int text_width, text_height;

	if (centered) {
		interfaces::surface->get_text_size(font, text, text_width, text_height);
		interfaces::surface->draw_text_pos(x - text_width / 2, y);
	}
	else
		interfaces::surface->draw_text_pos(x, y);

	interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_render_text(text, wcslen(text));
}

void render::text(std::int32_t x, std::int32_t y, unsigned long font, std::string text, bool centered, color color) {
	wchar_t temp[128];
	int text_width, text_height;
	if (MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, temp, 128) > 0) {
		interfaces::surface->draw_text_font(font);
		if (centered) {
			interfaces::surface->get_text_size(font, temp, text_width, text_height);
			interfaces::surface->draw_text_pos(x - text_width / 2, y);
		}
		else
			interfaces::surface->draw_text_pos(x, y);
		interfaces::surface->set_text_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_render_text(temp, wcslen(temp));
	}
}

void render::draw_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_filled_rect(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_filled_rectangle(x, y, width, height);
}

void render::draw_fade(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color1, color color2, bool horizontal) {
	interfaces::surface->set_drawing_color(color1.r, color1.g, color1.b, color1.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, color1.a, 0, horizontal);

	interfaces::surface->set_drawing_color(color2.r, color2.g, color2.b, color2.a);
	interfaces::surface->draw_filled_rect_fade(x, y, x + width, y + height, 0, color2.a, horizontal);
}

void render::draw_outline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->draw_outlined_rect(x, y, width, height);
}

void render::draw_fade_outline(std::int32_t x, std::int32_t y, std::int32_t width, std::int32_t height, color color) {
	//bottom
	draw_fade(x, y, width, 1, color, color::swap_color(color), true);
	//left
	draw_fade(x, y, 1, height, color, color::swap_color(color), false);

	draw_fade(x, y + height - 1, width, 1, color, color::swap_color(color), true);

	draw_fade(x + width - 1, y, 1, height, color, color::swap_color(color), false);
}

void render::draw_textured_polygon(std::int32_t vertices_count, vertex_t* vertices, color color) {
	static unsigned char buf[4] = { 255, 255, 255, 255 };
	unsigned int texture_id{};
	if (!texture_id) {
		texture_id = interfaces::surface->create_new_texture_id(true);
		interfaces::surface->set_texture_rgba(texture_id, buf, 1, 1);
	}
	interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
	interfaces::surface->set_texture(texture_id);
	interfaces::surface->draw_polygon(vertices_count, vertices);
}

void render::draw_circle(std::int32_t x, std::int32_t y, std::int32_t radius, std::int32_t segments, color color) {
	float step = M_PI * 2.0 / segments;
	for (float a = 0; a < (M_PI * 2.0); a += step) {
		float x1 = radius * cos(a) + x;
		float y1 = radius * sin(a) + y;
		float x2 = radius * cos(a + step) + x;
		float y2 = radius * sin(a + step) + y;
		interfaces::surface->set_drawing_color(color.r, color.g, color.b, color.a);
		interfaces::surface->draw_line(x1, y1, x2, y2);
	}
}

vec2_t render::get_text_size(unsigned long font, std::string text) {
	std::wstring a(text.begin(), text.end());
	const wchar_t* wstr = a.c_str();
	static int w, h;

	interfaces::surface->get_text_size(font, wstr, w, h);
	return { static_cast<float>(w), static_cast<float>(h) };
}

void render::draw_blur(std::int32_t x, std::int32_t y, std::int32_t w, std::int32_t h) {
	static i_material* mat = interfaces::material_system->find_material("dev/blurgaussian_3x3", TEXTURE_GROUP_OTHER, true);
	i_material_render_context* context = interfaces::material_system->get_render_context();
	int width, height;

	interfaces::engine->get_screen_size(width, height);
	context->draw_screen_space_rectangle(mat, x, y, w, h, x, y, x + w, y + h, width, height);
}

void render::convert_hsv_to_rgb(float h, float s, float v, float& out_r, float& out_g, float& out_b) {
	if (s == 0.0f) {
		out_r = out_g = out_b = v;
		return;
	}

	h = fmodf(h, 1.0f) / (60.0f / 360.0f);
	int   i = (int)h;
	float f = h - (float)i;
	float p = v * (1.0f - s);
	float q = v * (1.0f - s * f);
	float t = v * (1.0f - s * (1.0f - f));

	switch (i) {
	case 0: out_r = v; out_g = t; out_b = p; break;
	case 1: out_r = q; out_g = v; out_b = p; break;
	case 2: out_r = p; out_g = v; out_b = t; break;
	case 3: out_r = p; out_g = q; out_b = v; break;
	case 4: out_r = t; out_g = p; out_b = v; break;
	case 5: default: out_r = v; out_g = p; out_b = q; break;
	}
}

void render::convert_rgb_to_hsv(float& out_h, float& out_s, float& out_v, float& in_r, float& in_g, float& in_b) {
	float fCMax = max(max(in_r, in_g), in_b);
	float fCMin = min(min(in_r, in_g), in_b);
	float fDelta = fCMax - fCMin;

	if (fDelta > 0) {
		if (fCMax == in_r) {
			out_h = 60 * (fmod(((in_g - in_b) / fDelta), 6));
		}
		else if (fCMax == in_g) {
			out_h = 60 * (((in_b - in_r) / fDelta) + 2);
		}
		else if (fCMax == in_b) {
			out_h = 60 * (((in_r - in_g) / fDelta) + 4);
		}

		if (fCMax > 0) {
			out_s = fDelta / fCMax;
		}
		else {
			out_s = 0;
		}

		out_v = fCMax;
	}
	else {
		out_h = 0;
		out_s = 0;
		out_v = fCMax;
	}

	if (out_h < 0) {
		out_h = 360 + out_h;
	}
}