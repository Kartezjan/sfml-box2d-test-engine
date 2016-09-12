#include "source.h"

void update_and_render_all_renderable_objects(sf::RenderWindow& Window, std::vector<renderable_entity*>& entity_list) {
	for (size_t i = 0; i < entity_list.size(); ++i) {
		entity_list[i]->update();
		Window.draw(*entity_list[i]);
	}
}

void destroy_all_doomed_objects(std::queue<abstract_entity*>& death_queue) {
	while (!death_queue.empty()) {
		death_queue.front()->~abstract_entity();
		death_queue.pop();
	}
}

void main()
{

	//Prepare the window 
	sf::RenderWindow window(sf::VideoMode(1024, 768, 32), "Test");

	window.setFramerateLimit(60);

	//Prepare the viewpoint

	sf::View view = window.getDefaultView();
	window.setView(view);

	// Prepare textures
	resource_manager resources;
	sf::Texture ground_texture;
	sf::Texture box_texture;
	sf::Texture wheel_texture;
	sf::Texture car_body_texture;
	ground_texture.loadFromFile("ground.png");
	box_texture.loadFromFile("box.jpg");
	wheel_texture.loadFromFile("wheel.png");
	car_body_texture.loadFromFile("red_car.jpg");

	resources.textures.push_back(ground_texture);
	resources.textures.push_back(box_texture);
	resources.textures.push_back(wheel_texture);
	resources.textures.push_back(car_body_texture);

	// Prepare the world
	universe universe(b2Vec2(0.0f, 9.8f), resources);

	abstract_entity user_input;
	user_input.virtues.push_back(new produces_user_input(universe, window));

	abstract_entity camera;
	camera.virtues.push_back(new controlls_view(50, universe, view, window));
	camera.virtues.push_back(new tracks_object(universe, view, window));

	abstract_entity contact_handler;
	contact_handler.virtues.push_back(new handles_contacts(universe));

	abstract_entity nature;
	nature.virtues.push_back(new applies_force(universe));
	nature.virtues.push_back(new spawns_objects(universe));
	nature.virtues.push_back(new adds_to_death_queue(universe));

	//universe.physical_objects.push_back(new sprite_entity(ground_texture, sf::Vector2f(400.f, 8.f), create_ground2(universe.world, 400.f, 500.f), "ground"));
	universe.physical_objects.push_back(new physical_entity(create_ground(universe.world, 400.f + 12000.f, 500.f, 10000, 200), "ground", resources.textures[0]));
	universe.physical_objects.push_back(new physical_entity(create_ground(universe.world, 400.f, 500.f, 10000, 200), "ground", resources.textures[0]));
	universe.physical_objects.push_back(new physical_entity(create_ramp(universe.world, 400.f, 300.f), "ground", resources.textures[0]));

	sf::Font font;
	font.loadFromFile("C:\\Windows\\Fonts\\arial.ttf");
	sf::Text mouse_position_info;
	mouse_position_info.setFont(font);
	mouse_position_info.setString("mouse position - X: 0  Y: 0");
	mouse_position_info.setCharacterSize(12);
	mouse_position_info.setFillColor(sf::Color::Black);
	mouse_position_info.setPosition(sf::Vector2f(0, 0));


	image_entity mouse_pos_info_gui(&mouse_position_info, "GUI_MOUSE_POS", image_entity::TEXT, window);
	mouse_pos_info_gui.virtues.push_back(new changes_GUI_text(universe));

	universe.physical_objects.push_back(&mouse_pos_info_gui);

	physical_entity player(create_player(universe.world, 370, 480), "player", universe.resources.textures[3]);
	universe.physical_objects.push_back(&player);

	printf("%X\n", &player);

	physical_entity front_wheel(create_circle(universe.world, 370, 480, 26.f, 50.f, 0.7f), "front_wheel", universe.resources.textures[2]);
	physical_entity back_wheel(create_circle(universe.world, 370, 480, 26.f, 50.f, 0.7f), "back_wheel", universe.resources.textures[2]);
	universe.physical_objects.push_back(&front_wheel);
	universe.physical_objects.push_back(&back_wheel);
	setup_car(player, front_wheel, back_wheel, b2Vec2(64.f,-30.f), b2Vec2(-64.f,-30.f), 700, universe);
	player.virtues.push_back(new center_of_attention(universe));

	auto process_virtues = [](abstract_entity* target) {
		for (auto e : target->virtues) 
			e->send_message(target);
	};

	while (window.isOpen())
	{
		//Get input from the user
		process_virtues(&user_input);
		process_virtues(&camera);
		process_virtues(&contact_handler);
		//Invoke all physical entities' virtues (actions) 
		for (auto entity : universe.physical_objects)
			process_virtues(entity);
		// Apply entities' changes to the world
		process_virtues(&nature);

		universe.message_queues.check_all_lifetimes();
		universe.message_queues.delete_all_marked();

		destroy_all_doomed_objects(universe.death_queue);

		universe.world.Step(1/60.f, 8, 3);

		window.clear(sf::Color::White);
		update_and_render_all_renderable_objects(window, universe.physical_objects);
		window.display();
	}
}