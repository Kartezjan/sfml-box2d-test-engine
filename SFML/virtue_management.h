#pragma once

#include <Box2D\Box2D.h>
#include <SFML\Graphics.hpp>

#include "event_management.h"
#include "entities.h"

class abstract_entity;

class virtue {
public:
	virtual void send_message(abstract_entity* source, complete_message_storage& message_queue) = 0;
};

class controllable : public virtue {
public:
	void send_message(abstract_entity* source, complete_message_storage& message_queue);
	sf::Clock delta_time;
	size_t cooldown = 1200;
}; 

class produces_user_input : public virtue {
	void send_message(abstract_entity* source, complete_message_storage& message_queue);
};

class applies_force : public virtue {
	void send_message(abstract_entity* source, complete_message_storage& message_queue);
};