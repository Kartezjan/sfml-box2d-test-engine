#pragma once

#include "config.h"
#include "event_management.h"
#include "entities.h"
#include "physics.h"
#include "world.h"
#include "resource_manager.h"

enum class virtue_type
{ 
	EMPTY_VIRTUE,
	CONTROLLABLE, CONTROLLABLE_CAR,
	PRODUCES_USER_INPUT, 
	APPLIES_FORCE, SPAWNS_OBJECTS, 
	CHANGES_GUI_TEXT, CONTROLLS_VIEW, TRACKS_OBJECT, CENTER_OF_ATTENTION
};

const auto discard_all_messages = [&](auto& source, auto& a)
{
	if(a.fixture_a->GetBody() == source || a.fixture_b->GetBody() == source)
		a.delete_this_message = true;
};

class virtue {
public:
	explicit virtue(universe& uni_ref) : cosmos(uni_ref) {}
	virtual ~virtue() {};
	virtual void send_message(abstract_entity* source) = 0;
	
protected:
	universe& cosmos;
};

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