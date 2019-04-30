#pragma once

#include "virtue_management.h"

class processes_hp_messages : public virtue
{
public:
	using virtue::virtue;
	void send_message(abstract_entity* source) override;
};
