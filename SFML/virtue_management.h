#pragma once

#include "config.h"
#include "event_management.h"
#include "entities.h"
#include "physics.h"
#include "world.h"
#include "resource_manager.h"

class abstract_entity;

class virtue {
public:
	explicit virtue(universe& uni_ref) : cosmos(uni_ref) {}
	virtual void send_message(abstract_entity* source) = 0;
protected:
	universe& cosmos;
};

class controllable : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
private:
	size_t cooldown = 1200;
	size_t previous_timestamp = 0;
}; 

class produces_user_input : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class applies_force : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class spawns_objects : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
private:
	size_t cooldown = 300;
	size_t previous_creation_timestamp = 0;
	size_t previous_removal_timestamp = 0;
};