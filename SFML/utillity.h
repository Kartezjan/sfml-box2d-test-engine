#pragma once

#include "config.h"

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
};
