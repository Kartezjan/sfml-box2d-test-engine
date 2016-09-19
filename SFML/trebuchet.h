#pragma once

#include "physics.h"
#include "virtue_management.h"
#include "entities.h"

void setup_trebuchet(physical_entity& main_body, physical_entity& beam, /*physical_entity& projectile,*/ physical_entity& counter_weight, physical_entity& grand_wheel, b2Vec2 beam_pos, b2Vec2 counter_weight_pos, universe& cosmos);

class gear_controll : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
private:
	bool status = false;
	size_t previous_timestamp = 0;
};
