#include "source.h"

void update_and_render_all_objects(sf::RenderWindow& Window, std::vector<entity*>& entity_list) {
	for (size_t i = 0; i < entity_list.size(); ++i) {
		entity_list[i]->update();
		Window.draw(*entity_list[i]);
	}
}

void remove_objects(std::queue<entity*>& death_queue, b2World& world) {
	while (!death_queue.empty()) {
		auto current = death_queue.front();
		delete current;
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
	sf::RenderWindow Window(sf::VideoMode(800, 600, 32), "Test");
	Window.setFramerateLimit(60);

	/** Prepare textures */
	sf::Texture ground_texture;
	sf::Texture box_texture;
	ground_texture.loadFromFile("ground.png");
	box_texture.loadFromFile("box.jpg");

	/** Prepare the world */
	std::vector<entity*> physical_entities;

	abstract_entity user_input;
	user_input.virtues.push_back(new produces_user_input);

	abstract_entity physics;
	physics.virtues.push_back(new applies_force);

	b2Vec2 gravity(0.0f, 9.8f);
	b2World world(gravity);

	physical_entities.push_back(new sprite_entity(ground_texture, sf::Vector2f(400.f, 8.f), create_ground(world, 400.f, 500.f) ) );
	sprite_entity player(box_texture, sf::Vector2f(16.f, 16.f), create_box(world, 60, 60));
	player.virtues.push_back(new controllable);
	physical_entities.push_back(&player);

	auto process_virtues = [](abstract_entity* target, complete_message_storage& message_queue) {
		for (auto e : target->virtues) {
			e->send_message(target, message_queue);
		}
	};

	while (Window.isOpen())
	{
		std::queue<entity*> death_queue;
		complete_message_storage message_queues;

		process_virtues(&user_input, message_queues);

		for (auto e : physical_entities) 
			process_virtues(e, message_queues);

		process_virtues(&physics, message_queues);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && universe_clock.getElapsedTime() - last_box_creation >= cooldown)
		{
			int mouse_x = sf::Mouse::getPosition(Window).x;
			int mouse_y = sf::Mouse::getPosition(Window).y;
			physical_entities.push_back(new sprite_entity(box_texture, sf::Vector2f(16.f, 16.f), create_box(world, mouse_x, mouse_y)));
			last_box_creation = universe_clock.getElapsedTime();
		}

		if (sf::Mouse::isButtonPressed(sf::Mouse::Right) && universe_clock.getElapsedTime() - last_box_removal >= cooldown)
		{
			if (physical_entities.size() > 2) {
				death_queue.push(physical_entities.back());
				physical_entities.pop_back();
			}
			last_box_removal = universe_clock.getElapsedTime();
		}

		remove_objects(death_queue, world);
		world.Step(1 / 60.f, 8, 3);

		Window.clear(sf::Color::White);
		update_and_render_all_objects(Window, physical_entities);
		Window.display();
	}

	return 0;
}