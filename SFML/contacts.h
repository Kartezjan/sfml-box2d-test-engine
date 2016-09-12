#pragma once

#include "config.h"
#include "virtue_management.h"

class handles_contacts : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class destroys_upon_collision : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
};