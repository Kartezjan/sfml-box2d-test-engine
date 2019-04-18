#pragma once

#include "world.h"
#include "physics.h"
#include "virtue.h"
#include "entities.h"

void setup_car(physical_entity& body_car, physical_entity& wheel_front, physical_entity& wheel_back, b2Vec2 front_wheel_pos, b2Vec2 back_wheel_pos, float maxTorgue, universe& cosmos);

class controllable_car : public virtue {
public:
	controllable_car(universe& uni_ref, float n_acceleration) : virtue(uni_ref) { acceleration = n_acceleration; }
	void send_message(abstract_entity* source);
private:
	float acceleration;
	size_t previous_timestamp = 0;
};
