#pragma once

#include "world.h"
#include "physics.h"
#include "virtue.h"
#include "entities.h"

void setup_trebuchet(primitive_entity& main_body, primitive_entity& beam, primitive_entity& counter_weight, primitive_entity& grand_wheel, b2Vec2 beam_pos, b2Vec2 counter_weight_pos, universe& cosmos);

class gear_controll : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
private:
	bool status = false;
	size_t previous_timestamp = 0;
};
