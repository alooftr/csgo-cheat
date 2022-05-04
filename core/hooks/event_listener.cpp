#include "event_listener.hpp"
#include "../features/features.hpp"

event_manager event_listener;

void event_manager::fire_game_event(i_game_event* event) {
	
}

bool event_manager::initialize() {
	debug_id = EVENT_DEBUG_ID_INIT;
	interfaces::event_manager->add_listener(this, ("player_hurt"), false);;
	interfaces::event_manager->add_listener(this, ("player_death"), false);

	if (!interfaces::event_manager->find_listener(this, ("player_hurt")) || !interfaces::event_manager->find_listener(this, ("player_death"))){
		throw std::runtime_error(("failed to initialize events. (add_listener?)"));
		return false;
	}

	console::log("[event_listener]", "initialized");
	return true;
}

bool event_manager::release() {
	interfaces::event_manager->remove_listener(this);

	return true;
}