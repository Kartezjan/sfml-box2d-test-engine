#pragma once

#include "virtue.h"

struct body_properties;

class controllable : public virtue {
public:
	using virtue::virtue;
	void process() override; 
private:
	size_t cooldown = 1200;
	size_t previous_timestamp = 0;
}; 

class applies_force : public virtue {
public:
	using virtue::virtue;
	void process() override;
};

class spawns_objects : public virtue {
public:
	using virtue::virtue;
	void process() override;
private:
	entity_id spawn(const body_properties& what);
	int32 cooldown = 300;
	int32 previous_creation_timestamp = 0;
	int32 previous_removal_timestamp = 0;
	std::vector<entity_id> spawned_objects;
};

class destroys_all_doomed_objects : public virtue {
public:
	using virtue::virtue;
	void process() override;
};
