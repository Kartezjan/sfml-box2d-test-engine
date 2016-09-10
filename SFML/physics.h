#pragma once

#include "config.h"

struct body_properties {
	body_properties(b2World& world_ref) :
		world(world_ref)
	{}
	std::vector<b2FixtureDef> fixtures;
	b2BodyDef body_def;
	b2World& world;
};

b2Body* create_ground2(b2World& World, float X, float Y);

body_properties create_ground(b2World& World, float X, float Y, float width, float heigh);

b2Body* create_box(b2World& World, int MouseX, int MouseY);

b2Body* create_physical_body(body_properties&);

body_properties create_player(b2World& World, float X, float Y);

b2Vec2 getShapePosition(const b2Shape *shape);

body_properties create_circle(b2World& World, float X, float Y, float radius, float density, float friction);