#pragma once

#include "config.h"

#include "event_management.h"
#include "virtue_management.h"
#include "entities.h"
#include "physics.h"
#include "world.h"
#include "resource_manager.h"
#include "car.h"
#include "camera.h"
#include "GUI.h"
#include "user_input.h"
#include "contacts.h"

void update_and_render_scene(sf::RenderWindow& window, b2World& world);
void update_and_render_GUI_objects(sf::RenderWindow& window, std::vector<renderable_entity*> GUI_objects);