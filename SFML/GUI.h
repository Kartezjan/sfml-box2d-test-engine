#pragma once

#include "config.h"
#include "virtue_management.h"

class changes_GUI_text : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
};