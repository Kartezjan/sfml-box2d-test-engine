#pragma once

#include "config.h"
#include "entities.h"

void test_animation(resource_manager&, sf::RenderWindow& window);
void gui_test(universe& universe, sf::RenderWindow& window);
entity_id hero_test(universe& universe);
void make_obstacles(universe& universe);

void trebuchet_spawn(universe& universe);