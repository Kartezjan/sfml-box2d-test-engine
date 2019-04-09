#pragma once

#include "config.h"

#include "message_storage.h"


class abstract_entity;
class image_entity;

enum class force_type { APPLY_FORCE_TO_CENTER, APPLY_IMPULS_TO_CENTER };

enum class input_key { W, S, A, D, LMB, RMB, SPACE, LEFT, RIGHT, UP, DOWN, Z, X, B, E, Q};

enum class event_type { TRACKED_OBJECT_POS, FREE_CAMERA };

enum class move_direction { LEFT, RIGHT };

enum class contact_type { AABB_CONTACT, COLLISION };

struct message {
	bool delete_this_message = false;
	size_t lifetime = 1000; //in cycles
};

struct input_message : public message {
	input_key key;
};

struct force_message : public message {
	force_type type;
	b2Vec2 force;
	abstract_entity* source;
};

struct camera_message : public message {
	sf::Vector2f event_pos;
	event_type type;
	move_direction direction;
};

struct contact_message : public message {
	contact_message() : contact_type(::contact_type::COLLISION), fixture_a(nullptr), fixture_b(nullptr) {};
	contact_type contact_type;
	b2Fixture* fixture_a;
	b2Fixture* fixture_b;
};

struct death_message : public message {
	abstract_entity* target;
};

struct show_illusion_message : public message {
	image_entity* illusion_entity;
	size_t duration;
};

typedef message_storage<input_message, force_message, camera_message, contact_message, death_message, show_illusion_message> complete_message_storage;