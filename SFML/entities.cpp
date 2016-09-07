#include "entities.h"

abstract_entity::~abstract_entity() {
}

void sprite_entity::update() {
	sprite.setPosition(SCALE * physical_body->GetPosition().x, SCALE * physical_body->GetPosition().y);
	sprite.setRotation(physical_body->GetAngle() * 180 / b2_pi);
}

void sprite_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &texture;
	target.draw(sprite, states);
}

sprite_entity::sprite_entity(sf::Texture& box_texture, sf::Vector2f box_origin, b2Body* physics) {
	physical_body = physics;
	texture = box_texture;
	sprite.setTexture(texture);
	sprite.setOrigin(box_origin);
}

sprite_entity::~sprite_entity() {
	physical_body->GetWorld()->DestroyBody(physical_body);
}