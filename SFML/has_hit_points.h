#pragma once

#include "virtue_management.h"

class has_hit_points : public virtue
{
public:
	has_hit_points(universe& uni_ref, int hit_points) : virtue(uni_ref), hp_(hit_points) {}
	void send_message(abstract_entity* source) override;
private:
	int hp_;
};

