#include "physics.h"

b2Body* create_box(b2World& world, int mouse_x, int mouse_y)
{
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

b2Body* create_ground(b2World& world, float x, float y)
{
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