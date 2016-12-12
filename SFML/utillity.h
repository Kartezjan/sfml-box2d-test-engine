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

	template <class vec>
	b2Vec2 vec_to_b2vec(const vec vector) {
		return b2Vec2(vector.x / SCALE, vector.y / SCALE);
	}

	sf::Vector2f b2vec_to_sfvec(const b2Vec2 b2vec);
};
