#pragma once

#include "config.h"
#include "virtue.h"
#include "physics.h"

//enum class shape_type { CONVEX, CIRCLE };
enum class visual_effect_type { ROPE_JOINT };

struct visual_effect {
	visual_effect_type type;
	sf::Drawable* visual_object;
	float property;
};

class has_primitive_body : public virtue
{
	has_primitive_body(universe& universe_ref, _entity& entity_ref, body_properties& body_properties, const sf::Texture& box_texture)
		: virtue(universe_ref, entity_ref), texture(box_texture) { init(body_properties); }
	struct shape {
		shape_type type;
		sf::Drawable* visual_object;
	};
	void addVisualEffect(visual_effect effect) { visual_effects.push_back(effect); };
private:
	void init(body_properties& props);
	const sf::Texture& texture;
	std::vector<shape> shapes;
	std::vector<visual_effect> visual_effects;
};
