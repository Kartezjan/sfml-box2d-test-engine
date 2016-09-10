#pragma once

#include "config.h"

struct resource_manager;
class renderable_entity;

struct universe {
	universe(b2Vec2 gravity, resource_manager& res) :
		world(gravity),
		resources(res),
		mouse_pos(0, 0)
	{}
	b2World world;
	std::vector<renderable_entity*> physical_objects;
	std::queue<abstract_entity*> death_queue;
	complete_message_storage message_queues;
	sf::Clock universe_clock;
	sf::Vector2f mouse_pos;
	resource_manager& resources;
};