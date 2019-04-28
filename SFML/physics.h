#pragma once

#include "config.h"


class physical_entity;

enum class shape_type { CONVEX, CIRCLE };

struct shape {
	shape_type type;
	sf::Drawable* visual_object;
};

struct fixture_data
{
	enum type {standard = 0, foot};
	type fixture_type = standard;
	int contact_handler_data = 0;
};

struct body_properties
{
	body_properties(b2World& world_ref) :
		world(world_ref)
	{}
	std::vector<std::pair<fixture_data, b2FixtureDef>> fixtures;
	b2BodyDef body_def;
	b2World& world;
};


body_properties create_hero(b2World& world, float x, float y);
body_properties create_ranger(b2World& world, float x, float y);

body_properties create_ground(b2World& World, float X, float Y, float width, float heigh);

b2Body* create_physical_body(const body_properties&, physical_entity* target);

body_properties create_player(b2World& World, float X, float Y);

b2Vec2 get_shape_position(const b2Shape *shape);

body_properties create_box(b2World& world, int X, int Y);
body_properties create_box(b2World& world, int X, int Y, float height, float width, float density, float friction);
body_properties create_box(b2World& world, int X, int Y, float height, float width, b2Vec2 fixture_pos, float angle, float density, float friction);

body_properties create_kinematic_box(b2World& world, int X, int Y, float height, float width);

body_properties create_circle(b2World& World, float X, float Y, float radius, float density, float friction);

body_properties create_ramp(b2World& world, float X, float Y);

void apply_blast_impulse(b2Body* body, b2Vec2 blastCenter, b2Vec2 applyPoint, float blastPower);