#pragma once

#include "config.h"
#include "entity_map.h"
#include "resource_manager.h"
#include "event_management.h"

struct universe {
	universe(b2Vec2 gravity, resource_manager& res) :
		world(gravity),
		resources(res),
		mouse_pos(0, 0) {}
	b2World world;
	complete_message_storage message_queues;
	sf::Clock universe_clock;
	sf::Vector2f mouse_pos;
	resource_manager& resources;
	entity_map all_entities_;
};