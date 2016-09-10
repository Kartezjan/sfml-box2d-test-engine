#include "physics.h"

body_properties create_player(b2World& World, float X, float Y) {
	body_properties player_properties(World);
	b2BodyDef body_def;
	body_def.position = b2Vec2(X / SCALE, Y / SCALE);
	body_def.type = b2_dynamicBody;

	b2PolygonShape* shape = new b2PolygonShape;
	shape->SetAsBox((120.f / 2) / SCALE, (32.f / 2) / SCALE, b2Vec2(0.f / SCALE,-48.f / SCALE), 0);


	b2FixtureDef fixture_def;
	fixture_def.density = 1.f;
	fixture_def.friction = 0.7f;
	fixture_def.shape = shape;

	player_properties.body_def = body_def;
	player_properties.fixtures.push_back(fixture_def);

	return player_properties;
}

body_properties create_circle(b2World& World, float X, float Y, float radius, float density, float friction) {
	body_properties circle_properties(World);
	b2BodyDef body_def;
	body_def.position = b2Vec2(X / SCALE, Y / SCALE);
	body_def.type = b2_dynamicBody;

	b2CircleShape* shape_circle = new b2CircleShape;
	shape_circle->m_radius = radius / SCALE;

	b2FixtureDef fixture_def;
	fixture_def.density = density;
	fixture_def.friction = friction;
	fixture_def.shape = shape_circle;

	circle_properties.body_def = body_def;
	circle_properties.fixtures.push_back(fixture_def);
	return circle_properties;
}

body_properties create_ground(b2World& World, float X, float Y, float width, float heigh) {
	body_properties ground_properties(World);
	b2BodyDef body_def;
	body_def.position = b2Vec2(X / SCALE, Y / SCALE);
	body_def.type = b2_staticBody;

	b2PolygonShape* shape = new b2PolygonShape;
	shape->SetAsBox((width / 2) / SCALE, (heigh / 2) / SCALE, b2Vec2(0.f / SCALE, 0.f / SCALE), 0);

	b2FixtureDef fixture_def;
	fixture_def.density = 500.f;
	fixture_def.shape = shape;

	ground_properties.body_def = body_def;
	ground_properties.fixtures.push_back(fixture_def);

	return ground_properties;
}

b2Body* create_box(b2World& world, int mouse_x, int mouse_y) {
	b2BodyDef body_def;
	body_def.position = b2Vec2(mouse_x / SCALE, mouse_y / SCALE);
	body_def.type = b2_dynamicBody;
	b2Body* body = world.CreateBody(&body_def);

	b2PolygonShape shape;
	shape.SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef fixture_def;
	fixture_def.density = 1.f;
	fixture_def.friction = 0.7f;
	fixture_def.shape = &shape;
	body->CreateFixture(&fixture_def);
	return body;
}

b2Body* create_ground2(b2World& world, float x, float y) {
	b2BodyDef body_def;
	body_def.position = b2Vec2(x / SCALE, y / SCALE);
	body_def.type = b2_staticBody;
	b2Body* body = world.CreateBody(&body_def);

	b2PolygonShape shape;
	shape.SetAsBox((800.f / 2) / SCALE, (16.f / 2) / SCALE);
	b2FixtureDef fixture_def;
	fixture_def.density = 0.f;
	fixture_def.shape = &shape;
	body->CreateFixture(&fixture_def);
	return body;
}

b2Body* create_physical_body(body_properties& properties) {
	b2Body* body = properties.world.CreateBody(&properties.body_def);
	size_t i = 0;
	for (auto fixture : properties.fixtures)
		body->CreateFixture(&fixture);
	return body;
}

b2Vec2 getShapePosition(const b2Shape *shape) {
	if (shape->m_type == b2Shape::e_circle)
	{
		return static_cast<const b2CircleShape*>(shape)->m_p;
	}
	else if (shape->m_type == b2Shape::e_polygon)
	{
		return static_cast<const b2PolygonShape*>(shape)->m_centroid;
	}
	return b2Vec2_zero;
}