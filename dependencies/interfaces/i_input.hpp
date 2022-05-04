#pragma once
#include "../../source-sdk/classes/c_usercmd.hpp"
//Outdated
//class i_input {
//public:
//	char pad_0000[172]; //0x0000
//	bool N0000004E; //0x00AC
//	bool in_thirdperson; //0x00AD
//	char pad_00AE[2]; //0x00AE
//	vec3_t camera_offset; //0x00B0
//
//	c_usercmd* get_user_cmd(int slot, int sequence_num) {
//		using original_fn = c_usercmd * (__thiscall*)(void*, int, int);
//		return (*(original_fn**)this)[8](this, slot, sequence_num);
//	}
//};

class i_input {
public:
	std::byte           pad0[0xC];          // 0x0
	bool                track_ir_available; // 0xC
	bool                mouse_init;         // 0xD
	bool                mouse_active;       // 0xE
	std::byte           pad1[0x9A];         // 0xF
	bool                in_thirdperson;     // 0xA9
	std::byte           pad2[0x2];          // 0xAA
	vec3_t              camera_offset;      // 0xAC
	std::byte           pad3[0x38];         // 0xB8
	c_usercmd*			cmd;				// 0xF0

	c_usercmd* get_user_cmd(int slot, int sequence_num) {
		using original_fn = c_usercmd * (__thiscall*)(void*, int, int);
		return (*(original_fn**)this)[8](this, slot, sequence_num);
	}
};