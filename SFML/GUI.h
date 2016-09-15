#pragma once

#include "config.h"
#include "virtue_management.h"

class changes_GUI_text : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class shows_illusions : public virtue {
public:
	shows_illusions(universe& uni_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), window(win_ref) {}
	void send_message(abstract_entity* source);
private:
	struct illusion_data {
		image_entity* illusion_entity;
		size_t time_offset;
		size_t duration;
	};
	sf::RenderWindow& window;
	std::vector<illusion_data> illusions;
};