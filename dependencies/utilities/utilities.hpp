#pragma once
#include <cstdint>
#include "../../source-sdk/math/vector3d.hpp"

namespace utilities {
	inline std::uint8_t* alloc_key_values_client = nullptr;
	inline std::uint8_t* alloc_key_values_engine = nullptr;

	template<typename T, typename ...Args>
	constexpr auto call_virtual_method(void* class_base, int index, Args... args) {
		return ((*reinterpret_cast<T(__thiscall***)(void*, Args...)>(class_base))[index])(class_base, args...);
	}

	template< typename t >
	__forceinline static t call_virtual( void *name, int offset ) {
		int *table = *( int ** ) name;
		int address = table[ offset ];
		return ( t ) ( address );
	}

	std::uint8_t* pattern_scan(const char* module_name, const char* signature) noexcept;
}
