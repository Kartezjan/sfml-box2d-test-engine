#pragma once

#include <random>

#include "config.h"
#include "entity_map.h"

struct resource_manager;
class renderable_entity;

struct universe {
	universe(b2Vec2 gravity, resource_manager& res, sf::RenderWindow& win_ref) :
		world(gravity),
		resources(res),
		mouse_pos(0, 0),
		rng(0xFA7A1),
		win_ref(win_ref)
	{}
	b2World world;
	complete_message_storage message_queues;
	sf::Clock universe_clock;
	sf::Vector2f mouse_pos;
	resource_manager& resources;
	entity_map all_entities;
	bool editor_mode = false;
	entity_map gui_resources;
	std::default_random_engine rng;
	sf::RenderWindow& win_ref;
};