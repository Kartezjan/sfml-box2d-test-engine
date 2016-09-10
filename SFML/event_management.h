#pragma once

#include "config.h"

#include "message_storage.h"


class abstract_entity;

enum class force_type { APPLY_FORCE_TO_CENTER, APPLY_IMPULS_TO_CENTER };

enum class input_key { W, S, A, D, LMB, RMB, SPACE, LEFT, RIGHT, UP, DOWN, Z, X};

enum class event_type { TRACKED_OBJECT_POS, FREE_CAMERA };

enum class move_direction { LEFT, RIGHT };

struct message {
	bool delete_this_message = false;
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

typedef message_storage<input_message, force_message, camera_message> complete_message_storage;