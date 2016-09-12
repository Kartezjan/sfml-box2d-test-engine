#pragma once

#include "config.h"

class physical_entity;

struct body_properties {
	body_properties(b2World& world_ref) :
		world(world_ref)
	{}
	std::vector<b2FixtureDef> fixtures;
	b2BodyDef body_def;
	b2World& world;
};

body_properties create_ground(b2World& World, float X, float Y, float width, float heigh);

b2Body* create_physical_body(body_properties&, physical_entity* target);

body_properties create_player(b2World& World, float X, float Y);

b2Vec2 get_shape_position(const b2Shape *shape);

body_properties create_box(b2World& world, int X, int Y);

body_properties create_circle(b2World& World, float X, float Y, float radius, float density, float friction);

body_properties create_ramp(b2World& world, float X, float Y);