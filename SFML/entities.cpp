#include "entities.h"

abstract_entity::~abstract_entity() {
	if (physical_body != nullptr)
		physical_body->GetWorld()->DestroyBody(physical_body);
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

sprite_entity::sprite_entity(sf::Texture& box_texture, sf::Vector2f box_origin, b2Body* physics, std::string n_name) {
	name = n_name;
	physical_body = physics;
	texture = box_texture;
	sprite.setTexture(texture);
	sprite.setOrigin(box_origin);
}

sprite_entity::~sprite_entity() {
	physical_body->GetWorld()->DestroyBody(physical_body);
}

image_entity::~image_entity() {
	delete visual_object;
}

image_entity::image_entity(sf::Drawable* object, std::string n_name) {
	visual_object = object;
	name = n_name;
}

void image_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = 0;
	target.draw(*visual_object, states);
}

void image_entity::update() {

}

