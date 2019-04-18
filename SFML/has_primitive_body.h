#pragma once

#include "config.h"
#include "virtue.h"

enum class shape_type { CONVEX, CIRCLE };
enum class visual_effect_type { ROPE_JOINT };

class has_primitive_body : public virtue
{
	has_primitive_body(universe& universe_ref, _entity& entity_ref, b2Body* definition, ) : virtue(universe_ref, entity_ref)
	{
	}
	struct shape {
		shape_type type;
		sf::Drawable* visual_object;
	};
	void addVisualEffect(visual_effect effect) { visual_effects.push_back(effect); };
	primitive_entity(body_properties& body_properties, std::string n_name, const sf::Texture& box_texture);
	~primitive_entity();
	void update();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	const sf::Texture& texture;
	std::vector<shape> shapes;
	std::vector<visual_effect> visual_effects;
};