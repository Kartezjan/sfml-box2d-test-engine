#pragma once

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>
#include <vector>

#include "config.h"

class virtue;

class abstract_entity {
public:
	~abstract_entity();
	std::vector<virtue*> virtues;
	b2Body* physical_body = nullptr; //an abstract entity has no physical body.
};

class entity : public abstract_entity, public sf::Drawable, public sf::Transformable {
public:
	virtual void update(void) = 0;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class sprite_entity : public entity {
public:
	sprite_entity(sf::Texture& box_texture, sf::Vector2f box_origin, b2Body* physics);
	~sprite_entity();

private:
	void update();
	void draw(sf::RenderTarget& target, sf::RenderStates states) const;

	sf::Texture texture;
	sf::Sprite sprite;
};