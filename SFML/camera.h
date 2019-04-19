#pragma once

#include "config.h"
#include "virtue.h"
#include "world.h"

class controls_view : public virtue {
public:
	controls_view(universe& uni_ref, _entity& entity_ref, float scroll_speed, sf::View& view_ref, sf::RenderWindow& win_ref) : virtue(uni_ref, entity_ref), view_(view_ref), window_ref_(win_ref) { speed_ = scroll_speed; }
	void process();
private:
	sf::View& view_;
	sf::RenderWindow& window_ref_;
	float speed_;
	float current_scale_ = 1.0f;
	float delta_scale_ = 0.05f;
	float max_scale_ = 2.0f;
	float min_scale_ = 0.2f;
};

class tracks_object : public virtue {
public:
	tracks_object(universe& uni_ref, _entity& owner_ref,  sf::View& view_ref, sf::RenderWindow& win_ref) : virtue(uni_ref, owner_ref), view_(view_ref), window_ref_(win_ref) {}
	void process();
private:
	bool free_mode_ = false;
	sf::View& view_;
	sf::RenderWindow& window_ref_;
};

class center_of_attention : public virtue {
public:
	using virtue::virtue;
	void process();
};