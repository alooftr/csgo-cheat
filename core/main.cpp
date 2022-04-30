#include "../dependencies/utilities/csgo.hpp"
#include "features/features.hpp"
#include "../dependencies/utilities/utilities.hpp"
#include "menu/menu.hpp"
#include "menu/config/config.hpp"

unsigned long WINAPI initialize(void* instance) {
	while (!GetModuleHandleA("serverbrowser.dll"))
		Sleep(200);

#ifdef _DEBUG
	console::initialize("csgo-cheat console");
#endif

	try {
		interfaces::initialize();
		render::initialize();
		hooks::initialize();
		config::initialize();
		console::push_color(FOREGROUND_INTENSE_MAGENTA);
		console::log("[info]", "csgo-cheat - build date: %s / %s", __DATE__, __TIME__);
		console::pop_color();
		interfaces::console->console_color_printf(Color((int)(variables::menu_clr[0] * 255), (int)(variables::menu_clr[1] * 255), (int)(variables::menu_clr[2] * 255), 255), "[csgo-cheat] - build date: %s / %s\n", __DATE__, __TIME__);
	}

	catch (const std::runtime_error & error) {
		MessageBoxA(nullptr, error.what(), "csgo-cheat error!", MB_OK | MB_ICONERROR);
		FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
	}

	while (!GetAsyncKeyState(VK_END))
		std::this_thread::sleep_for(std::chrono::milliseconds(50));

	FreeLibraryAndExitThread(static_cast<HMODULE>(instance), 0);
}

unsigned long WINAPI release() {
	hooks::release();

#ifdef _DEBUG
	console::release();
#endif

	return TRUE;
}

std::int32_t WINAPI DllMain(const HMODULE instance [[maybe_unused]], const unsigned long reason, const void* reserved [[maybe_unused]] ) {
	DisableThreadLibraryCalls(instance);

	switch (reason) {
	case DLL_PROCESS_ATTACH: {
		if (auto handle = CreateThread(nullptr, NULL, initialize, instance, NULL, nullptr))
			CloseHandle(handle);

		break;
	}

	case DLL_PROCESS_DETACH: {
		release();
		break;
	}
	}

	return true;
}
