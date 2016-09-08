#pragma once

#include "config.h"

#include "event_management.h"
#include "virtue_management.h"
#include "entities.h"
#include "physics.h"
#include "world.h"
#include "resource_manager.h"


void update_and_render_all_objects(sf::RenderWindow& Window, std::vector<entity*>& entity_list);
void remove_objects(std::queue<entity*>& death_queue, b2World& world);