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
public:
	produces_user_input(universe& uni_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), window(win_ref) {}
	void send_message(abstract_entity* source);
private:
	sf::RenderWindow& window;
	size_t previous_timestamp = 0;
};

class applies_force : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class spawns_objects : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
private:
	size_t cooldown = 300;
	size_t previous_creation_timestamp = 0;
	size_t previous_removal_timestamp = 0;
};

class changes_GUI_text : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class controlls_view : public virtue {
public:
	controlls_view(float scrollspeed, universe& uni_ref, sf::View& view_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), view(view_ref), window(win_ref) { speed = scrollspeed; }
	void send_message(abstract_entity* source);
private:
	sf::View& view;
	sf::RenderWindow& window;
	float speed;
	float current_scale = 1.0f;
	float delta_scale = 0.05f;
	float max_scale = 2.0f;
	float min_scale = 0.2f;
};

class tracks_object : public virtue {
public:
	tracks_object(universe& uni_ref, sf::View& view_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), view(view_ref), window(win_ref) {}
	void send_message(abstract_entity* source);
private:
	bool free_mode = false;
	sf::View& view;
	sf::RenderWindow& window;
};

class center_of_attention : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
};