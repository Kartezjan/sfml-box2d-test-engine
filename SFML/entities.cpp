#include "entities.h"

abstract_entity::~abstract_entity() {
	if (physical_body != nullptr)
		physical_body->GetWorld()->DestroyBody(physical_body);
	for (auto virtue : virtues)
		delete virtue;
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
	abstract_entity::~abstract_entity();
}

image_entity::~image_entity() {
	abstract_entity::~abstract_entity();
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

physical_entity::physical_entity(body_properties& body_properties, std::string n_name, sf::Texture& box_texture) {
	texture = box_texture;
	name = n_name;
	physical_body = create_physical_body(body_properties);
	for (auto fixture : body_properties.fixtures) {
		if (fixture.shape->m_type == fixture.shape->e_polygon) {
			const b2PolygonShape shape = *(b2PolygonShape*)fixture.shape;
			sf::ConvexShape convex;
			convex.setPointCount(shape.m_count);
			for (size_t i = 0; i < shape.m_count; ++i)
				convex.setPoint(i, sf::Vector2f(shape.m_vertices[i].x * SCALE, shape.m_vertices[i].y * SCALE));
			convex.setOrigin(sf::Vector2f(shape.m_centroid.x * SCALE, shape.m_centroid.y * SCALE));
			convex.setTexture(&texture);
			convexes.push_back(convex);
		}
		else {
			sf::CircleShape circle;
			const b2CircleShape shape = *(b2CircleShape*)fixture.shape;
			circle.setRadius(shape.m_radius * SCALE);
			circle.setOrigin(sf::Vector2f(shape.m_radius * SCALE,shape.m_radius * SCALE));
			circle.setTexture(&texture);
			circles.push_back(circle);
		}
	}
	//rendered_convexes.resize(convexes.size());
	//rendered_circles.resize(circles.size());
}

void physical_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &texture;
	for (auto convex : convexes)
		target.draw(convex, states);
	for (auto circle : circles)
		target.draw(circle, states);
}

void physical_entity::update() {
	size_t convex_count = 0;
	size_t circle_count = 0;

	for (auto fixture = physical_body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		auto pos = getShapePosition(fixture->GetShape());
		auto body_pos = physical_body->GetPosition();
		pos += body_pos;
		helper::rotate(pos, b2Vec2(body_pos.x, body_pos.y), physical_body->GetAngle());
		if (fixture->GetShape()->m_type == fixture->GetShape()->e_polygon) {
			convexes[convex_count].setPosition(sf::Vector2f(pos.x * SCALE, pos.y * SCALE));
			convexes[convex_count].setRotation(physical_body->GetAngle() * 180 / b2_pi);
			++convex_count;
		}
		else {
			circles[circle_count].setPosition(sf::Vector2f(pos.x * SCALE, pos.y * SCALE));
			circles[circle_count].setRotation(physical_body->GetAngle() * 180 / b2_pi);
			++circle_count;
		}
	}
}


physical_entity::~physical_entity() {
	abstract_entity::~abstract_entity();
}
