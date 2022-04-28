#pragma once
#include <cstdint>
#include <d3d9.h>
#include <math.h>

struct color {
	int a, r, g, b;
	color( ) = default;
	color( const int r, const int g, const int b, const int a = 255 ) {
		this->r = r;
		this->g = g;
		this->b = b;
		this->a = a;
	}

	void set_color(const int _r, const int _g, const int _b, const int _a = 255)
	{
		this->r = (unsigned char)_r;
		this->g = (unsigned char)_g;
		this->b = (unsigned char)_b;
		this->a = (unsigned char)_a;
	}

	explicit color( const uint32_t color ) {
		this->a = ( color >> 24 ) & 0xff;
		this->r = ( color >> 16 ) & 0xff;
		this->g = ( color >> 8 ) & 0xff;
		this->b = ( color & 0xff );
	}

	static color from_uint( const uint32_t uint ) {
		return color( uint );
	}

	static D3DCOLOR from_color( const color col ) {
		return D3DCOLOR_ARGB( col.a, col.r, col.g, col.b );
	}

	static color black( const int a = 255 ) { return { 0, 0, 0, a }; }
	static color white( const int a = 255 ) { return { 255, 255, 255, a }; }
	static color red( const int   a = 255 ) { return { 255, 0, 0, a }; }
	static color green( const int a = 255 ) { return { 0, 255, 0, a }; }
	static color blue( const int  a = 255 ) { return { 0, 0, 255, a }; }
	static color yellow(int a = 255) { return  { 247, 202, 24, a }; }

	static color menu(const color clr = color (55, 135, 235, 255)) { return { clr.r, clr.g, clr.b, clr.a }; }

	static color rainbow(float realtime, const int speed = 1, const  int a = 255) {
		return color(sin(realtime * speed) * 127 + 128, 
			         sin(realtime * speed + 2) * 127 + 128, 
					 sin(realtime * speed + 4) * 127 + 128, a);
	};

	static color swap_color(const color clr) { return color(clr.b, clr.r, clr.g); };

	static int breathing(float realtime, int speed = 1) { return sin(realtime * speed) * 127 + 128; }
};
