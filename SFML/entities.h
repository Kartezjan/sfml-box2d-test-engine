#pragma once

#include "config.h"
#include "utillity.h"

#include "virtue_management.h"
#include "physics.h"

class virtue;

enum class entity_type {ABSTRACT, IMAGE, PHYSICAL, OTHER};

class abstract_entity {
public:
	~abstract_entity();
	std::string name;
	std::vector<virtue*> virtues;
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
	enum content_type { TEXT };
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

class sprite_entity : public renderable_entity {
public:
	sprite_entity(sf::Texture& box_texture, sf::Vector2f box_origin, b2Body* physics, std::string name);
	~sprite_entity();
	void update();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Texture texture;
	sf::Sprite sprite;
};

class physical_entity : public renderable_entity {
public:
	physical_entity(body_properties& body_properties, std::string n_name, sf::Texture& box_texture);
	~physical_entity();
	void update();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Texture texture;
	std::vector<sf::ConvexShape> convexes;
	std::vector<sf::CircleShape> circles;
};