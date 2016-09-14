#include "physics.h"

body_properties create_ramp(b2World& world, float X, float Y) {
	body_properties ramp_properties(world);
	b2BodyDef body_def;
	body_def.position = b2Vec2(X / SCALE, Y / SCALE);
	body_def.type = b2_staticBody;

	b2Vec2 vertices[3];
	vertices[0] = b2Vec2( (5000 - 400) / SCALE, 100 / SCALE);
	vertices[1] = b2Vec2(5000 / SCALE, 100 / SCALE);
	vertices[2] = b2Vec2(5000 / SCALE, -60 / SCALE);
	b2PolygonShape* shape = new b2PolygonShape;
	shape->Set(vertices, 3);

	b2FixtureDef fixture_def;
	fixture_def.density = 500.f;
	fixture_def.shape = shape;

	ramp_properties.body_def = body_def;
	ramp_properties.fixtures.push_back(fixture_def);

	return ramp_properties;
}

body_properties create_player(b2World& world, float X, float Y) {
	body_properties player_properties(world);
	b2BodyDef body_def;
	body_def.position = b2Vec2(X / SCALE, Y / SCALE);
	body_def.type = b2_dynamicBody;

	player_properties.body_def = body_def;

	b2Vec2 vertices[4];
	vertices[0] = b2Vec2(-120 / SCALE, -20 / SCALE);
	vertices[1] = b2Vec2(120 / SCALE, -20 / SCALE);
	vertices[2] = b2Vec2(120 / SCALE, -40 / SCALE);
	vertices[3] = b2Vec2(-120 / SCALE, -40 / SCALE);

	b2PolygonShape* shape = new b2PolygonShape;
	shape->Set(vertices, 4);

	vertices[0] = b2Vec2(-120 / SCALE, -20 / SCALE);
	vertices[1] = b2Vec2(-100 / SCALE, -20 / SCALE);
	vertices[2] = b2Vec2(-100 / SCALE, -100 / SCALE);
	vertices[3] = b2Vec2(-120 / SCALE, -100 / SCALE);

	b2PolygonShape* shape2 = new b2PolygonShape;
	shape2->Set(vertices, 4);


	vertices[0] = b2Vec2(100 / SCALE, -20 / SCALE);
	vertices[1] = b2Vec2(120 / SCALE, -20 / SCALE);
	vertices[2] = b2Vec2(120 / SCALE, -100 / SCALE);
	vertices[3] = b2Vec2(100 / SCALE, -100 / SCALE);

	b2PolygonShape* shape3 = new b2PolygonShape;
	shape3->Set(vertices, 4);

	b2FixtureDef fixture_def;
	fixture_def.density = 30.f;
	fixture_def.friction = 0.7f;
	fixture_def.shape = shape;
	player_properties.fixtures.push_back(fixture_def);

	fixture_def.shape = shape2;
	player_properties.fixtures.push_back(fixture_def);

	fixture_def.shape = shape3;
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
	fixture_def.restitution = 0.4f;
	fixture_def.shape = shape_circle;

	circle_properties.body_def = body_def;
	circle_properties.fixtures.push_back(fixture_def);
	return circle_properties;
}

body_properties create_ground(b2World& world, float X, float Y, float width, float heigh) {
	body_properties ground_properties(world);
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

body_properties create_box(b2World& world, int X, int Y) {
	body_properties box_properties(world);
	b2BodyDef body_def;
	body_def.position = b2Vec2(X / SCALE, Y / SCALE);
	body_def.type = b2_dynamicBody;

	b2PolygonShape* shape = new b2PolygonShape;
	shape->SetAsBox((32.f / 2) / SCALE, (32.f / 2) / SCALE);
	b2FixtureDef fixture_def;
	fixture_def.density = 1.f;
	fixture_def.friction = 0.7f;
	fixture_def.shape = shape;

	box_properties.body_def = body_def;
	box_properties.fixtures.push_back(fixture_def);

	return box_properties;
}

b2Body* create_physical_body(body_properties& properties, physical_entity* target) {
	b2Body* body = properties.world.CreateBody(&properties.body_def);
	size_t i = 0;
	for (auto fixture : properties.fixtures)
		body->CreateFixture(&fixture);
	body->SetUserData(target);
	return body;
}

b2Vec2 get_shape_position(const b2Shape *shape) {
	if (shape->m_type == b2Shape::e_circle)
	{
		return static_cast<const b2CircleShape*>(shape)->m_p;
	}
	if (shape->m_type == b2Shape::e_polygon)
	{
		return static_cast<const b2PolygonShape*>(shape)->m_centroid;
	}
	return b2Vec2_zero;
}