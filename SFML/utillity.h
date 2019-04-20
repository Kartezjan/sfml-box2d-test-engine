#pragma once

#include "config.h"
#include "physics.h"

namespace utillity {
	template <class vec, class d>
	vec& rotate(vec& v, const vec& origin, d angle) {
		auto s = sin(angle);
		auto c = cos(angle);
		vec rotated;

		v -= origin;

		rotated.x = v.x * c - v.y * s;
		rotated.y = v.x * s + v.y * c;

		return v = (rotated + origin);
	}

	std::vector<shape> convert_shape_to_sf(const body_properties&, const sf::Texture&);
};
