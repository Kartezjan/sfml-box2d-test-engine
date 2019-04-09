#pragma once

#include "config.h"
#include "utillity.h"

#include "virtue_management.h"
#include "physics.h"

class virtue;

enum class entity_type {ABSTRACT, IMAGE, PHYSICAL, OTHER};
enum class shape_type { CONVEX, CIRCLE };
enum class visual_effect_type { ROPE_JOINT };

struct visual_effect {
	visual_effect_type type;
	sf::Drawable* visual_object;
	float property;
};

class abstract_entity {
public:
	~abstract_entity();
	std::string name;
	std::vector<std::unique_ptr<virtue>> virtues;
	entity_type get_type() { return type; }
	b2Body* get_physical_body() { return physical_body; };
protected:
	entity_type type = entity_type::ABSTRACT;
	b2Body* physical_body = nullptr; //an abstract entity has no physical body.
};

class renderable_entity : public abstract_entity, public sf::Drawable, public sf::Transformable {
public:
	virtual void update(void) = 0;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class image_entity : public renderable_entity {
public:
	enum content_type { TEXT, ILLUSION, ROPE };
	image_entity(sf::Drawable* object, std::string n_name, content_type n_type, sf::RenderWindow& win_ref);
	~image_entity();
	void update(void);
	sf::Drawable* get_visual_object() { return visual_object; }
	content_type get_image_type() { return image_type; };
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Drawable* visual_object;
	content_type image_type;
	sf::RenderWindow& window;
};

class physical_entity : public renderable_entity {
public:
	struct shape {
		shape_type type;
		sf::Drawable* visual_object;
	};
	void addVisualEffect(visual_effect effect) { visual_effects.push_back(effect); };
	physical_entity(body_properties& body_properties, std::string n_name, sf::Texture& box_texture);
	~physical_entity();
	void update();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Texture texture;
	std::vector<shape> shapes;
	std::vector<visual_effect> visual_effects;
};