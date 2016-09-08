#pragma once

#include "config.h"

class virtue;

class abstract_entity {
public:
	~abstract_entity();
	std::vector<virtue*> virtues;
	b2Body* physical_body = nullptr; //an abstract entity has no physical body.
	std::string name;
};

class entity : public abstract_entity, public sf::Drawable, public sf::Transformable {
public:
	virtual void update(void) = 0;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class image_entity : public entity {
public:
	image_entity(sf::Drawable* object, std::string n_name);
	~image_entity();
	void update(void);
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;
	sf::Drawable* visual_object;
};

class sprite_entity : public entity {
public:
	sprite_entity(sf::Texture& box_texture, sf::Vector2f box_origin, b2Body* physics, std::string name);
	~sprite_entity();
	void update();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture texture;
	sf::Sprite sprite;
};