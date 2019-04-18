#pragma once

#include "virtue.h"
#include "world.h"

class controllable : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source) override; 
private:
	size_t cooldown = 1200;
	size_t previous_timestamp = 0;
}; 

class applies_force : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source) override;
};

class spawns_objects : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source) override;
private:
	int32 cooldown = 300;
	int32 previous_creation_timestamp = 0;
	int32 previous_removal_timestamp = 0;
	std::vector<std::unique_ptr<physical_entity>> spawned_objects;
};

class destroys_all_doomed_objects : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source) override;
};
