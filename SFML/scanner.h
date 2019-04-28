#pragma once

#include <set>

#include "virtue_management.h"

class scanner
{
public:
	scanner
	(
		universe& uni_ref,
		const float delta,
		const float range
	) : 
		delta_(delta),
		range_(range),
		cosmos_(uni_ref) {}
	auto scan(physical_entity* source, const std::pair<float, float> angle_range, const b2Vec2 offset, const bool sight_mode = false) const->std::set<entity_id>;
private:
	float delta_;
	float range_;
	universe& cosmos_;
};
