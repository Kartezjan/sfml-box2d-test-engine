#pragma once

#include "config.h"
#include "virtue_management.h"

class controlls_view : public virtue {
public:
	controlls_view(float scrollspeed, universe& uni_ref, sf::View& view_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), view(view_ref), window(win_ref)
	{ 
		speed = scrollspeed;
		sf::Vector2u view_size = window.getSize();
		view_size.x *= current_scale;
		view_size.y *= current_scale;
		view.setSize(sf::Vector2f(view_size.x, view_size.y));
		window.setView(view);
	}
	void send_message(abstract_entity* source);
private:
	sf::View& view;
	sf::RenderWindow& window;
	float speed;
	float current_scale = 1.6f;
	float delta_scale = 0.05f;
	float max_scale = 2.0f;
	float min_scale = 0.2f;
};

class tracks_object : public virtue {
public:
	tracks_object(universe& uni_ref, sf::View& view_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), view(view_ref), window(win_ref) {}
	void send_message(abstract_entity* source);
private:
	bool free_mode = false;
	sf::View& view;
	sf::RenderWindow& window;
};

class center_of_attention : public virtue {
	using virtue::virtue;
	void send_message(abstract_entity* source);
};