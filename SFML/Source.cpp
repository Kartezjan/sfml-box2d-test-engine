#include "source.h"

void update_and_render_all_objects(sf::RenderWindow& Window, std::vector<entity*>& entity_list) {
	for (size_t i = 0; i < entity_list.size(); ++i) {
		entity_list[i]->update();
		Window.draw(*entity_list[i]);
	}
}

std::string mouse_cords_info(sf::RenderWindow& window) {
	char buffer[100];
	std::string text = "mouse position - X:";
	_itoa_s(sf::Mouse::getPosition(window).x, buffer, 10);
	text.append(buffer);
	text.append("  Y:");
	_itoa_s(sf::Mouse::getPosition(window).y, buffer, 10);
	text.append(buffer);
	return text;
}

void destroy_all_doomed_objects(std::queue<abstract_entity*>& death_queue) {
	while (!death_queue.empty()) {
		death_queue.front()->~abstract_entity();
		death_queue.pop();
	}
}

int main()
{
	sf::Time cooldown(sf::milliseconds(300));
	sf::Time last_box_creation(sf::Time::Zero);
	sf::Time last_box_removal(sf::Time::Zero);
	sf::Clock universe_clock;

	/** Prepare the window */
	sf::RenderWindow window(sf::VideoMode(800, 600, 32), "Test");
	window.setFramerateLimit(60);

	/** Prepare textures */
	resource_manager resources;
	sf::Texture ground_texture;
	sf::Texture box_texture;
	ground_texture.loadFromFile("ground.png");
	box_texture.loadFromFile("box.jpg");
	resources.textures.push_back(ground_texture);
	resources.textures.push_back(box_texture);

	/** Prepare the world */
	universe universe(b2Vec2(0.0f, 9.8f), resources);
	//std::vector<entity*> physical_entities;

	abstract_entity user_input;
	user_input.virtues.push_back(new produces_user_input(universe));

	abstract_entity nature;
	nature.virtues.push_back(new applies_force(universe));
	nature.virtues.push_back(new spawns_objects(universe));

	universe.physical_objects.push_back(new sprite_entity(ground_texture, sf::Vector2f(400.f, 8.f), create_ground(universe.world, 400.f, 500.f), "ground"));
	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text mouse_position_info;
	mouse_position_info.setFont(font);
	mouse_position_info.setString("mouse position - X: 0  Y: 0");
	mouse_position_info.setCharacterSize(12);
	mouse_position_info.setFillColor(sf::Color::Black);
	mouse_position_info.setPosition(sf::Vector2f(0, 0));
	universe.physical_objects.push_back(new image_entity(&mouse_position_info, "GUI_MOUSE_POS"));
	sprite_entity player(box_texture, sf::Vector2f(16.f, 16.f), create_box(universe.world, 60, 60), "player");
	player.virtues.push_back(new controllable(universe));
	universe.physical_objects.push_back(&player);

	auto process_virtues = [&universe_clock](abstract_entity* target) {
		for (auto e : target->virtues) 
			e->send_message(target);
	};

	while (window.isOpen())
	{
		universe.mouse_pos = sf::Mouse::getPosition(window);
		mouse_position_info.setString(mouse_cords_info(window));
		process_virtues(&user_input);
		for (auto e : universe.physical_objects)
			process_virtues(e);
		process_virtues(&nature);
		universe.message_queues = complete_message_storage();
		universe.world.Step(1 / 60.f, 8, 3);
		destroy_all_doomed_objects(universe.death_queue);
		window.clear(sf::Color::White);
		update_and_render_all_objects(window, universe.physical_objects);
		window.display();
	}

	return 0;
}