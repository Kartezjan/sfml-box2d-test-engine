#include "utillity.h"

namespace utillity {
	sf::Vector2f b2vec_to_sfvec(const b2Vec2 b2vec) {
		return sf::Vector2f(b2vec.x * SCALE, b2vec.y * SCALE);
	}
}