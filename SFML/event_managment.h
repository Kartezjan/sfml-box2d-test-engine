#pragma once


#include <string>
#include <Box2D\Box2D.h>

#include "message_storage.h"


class abstract_entity;

enum class force_type { APPLY_TO_CENTER };

struct message {
	bool delete_this_message = false;
};

struct keyboard_input_message : public message {
	std::string key;
};

struct force_message : public message {
	force_type type;
	b2Vec2 force;
	abstract_entity* source;
};

typedef message_storage<keyboard_input_message, force_message> complete_message_storage;