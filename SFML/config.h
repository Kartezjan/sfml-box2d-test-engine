#pragma once

#include <vector>
#include <string>
#include <queue>

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

#define SCALE 30.f

namespace helper {
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
