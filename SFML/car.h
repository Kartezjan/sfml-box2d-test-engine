#pragma once

#include "world.h"
#include "virtue.h"

void setup_car(_entity& body_car_entity, _entity& wheel_front_entity, _entity& wheel_back_entity, b2Vec2 front_wheel_pos, b2Vec2 back_wheel_pos, float maxTorgue, universe& cosmos);

class controllable_car : public virtue {
public:
	controllable_car(universe& uni_ref, _entity& owner_ref, float n_acceleration) : virtue(uni_ref, owner_ref), acceleration_(n_acceleration) {}
	void process() override;
private:
	float acceleration_;
	size_t previous_timestamp_ = 0;
};
