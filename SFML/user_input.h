#pragma once

#include "config.h"
#include "virtue_management.h"

class produces_user_input : public virtue {
public:
	produces_user_input(universe& uni_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), window(win_ref) {}
	void send_message(abstract_entity* source);
private:
	sf::RenderWindow& window;
	size_t previous_timestamp = 0;
};