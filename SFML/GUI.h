#pragma once

#include "config.h"
#include "virtue_management.h"

void setup_gui(std::vector<renderable_entity*>& container, sf::RenderWindow& win_ref, universe& uni_ref);

class changes_GUI_text : public virtue {
public:
	using virtue::virtue;
	void send_message(abstract_entity* source);
};

class updates_hit_points_status  : public virtue {
public:
	updates_hit_points_status(universe& uni_ref, entity_id target) : virtue(uni_ref), target_(target) {}
	void send_message(abstract_entity* source) override;
	void change_target(entity_id target) { target_ = target; }
private:
	entity_id target_;
};
class shows_illusions : public virtue {
public:
	shows_illusions(universe& uni_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), window(win_ref) {}
	void send_message(abstract_entity* source);
private:
	struct illusion_data {
		std::unique_ptr<image_entity> illusion_entity;
		int32 time_offset;
		int32 duration;
	};
	sf::RenderWindow& window;
	std::vector<illusion_data> illusions;
};