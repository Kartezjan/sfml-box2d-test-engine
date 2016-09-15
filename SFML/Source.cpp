#include "source.h"

void update_and_render_GUI_objects(sf::RenderWindow& window, std::vector<renderable_entity*> GUI_objects) {
	for(auto obj : GUI_objects) {
		obj->update();
		window.draw(*obj);
	}
}

void update_and_render_scene(sf::RenderWindow& window, b2World& world) {
	for (auto current = world.GetBodyList(); current; current = current->GetNext() ) {
		auto current_entity = (renderable_entity*)current->GetUserData();
		current_entity->update();
		window.draw(*current_entity);
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
	nature.virtues.push_back(new destroys_all_doomed_objects(universe));

	std::vector<physical_entity*> ground_objects;

	ground_objects.push_back(new physical_entity(create_ground(universe.world, 400.f + 12000.f, 500.f, 10000, 200), "ground", resources.textures[0]));
	ground_objects.push_back(new physical_entity(create_ground(universe.world, 400.f, 500.f, 10000, 200), "ground", resources.textures[0]));
	ground_objects.push_back(new physical_entity(create_ramp(universe.world, 400.f, 300.f), "ground", resources.textures[0]));


	physical_entity player(create_player(universe.world, 370, 350), "player", universe.resources.textures[3]);

	physical_entity front_wheel(create_circle(universe.world, 370, 350, 20.f, 10.f, 0.7f), "front_wheel", universe.resources.textures[2]);
	physical_entity back_wheel(create_circle(universe.world, 370, 350, 20.f, 10.f, 0.7f), "back_wheel", universe.resources.textures[2]);
	setup_car(player, front_wheel, back_wheel, b2Vec2(-100.f,-10.f), b2Vec2(100.f,-10.f), 500, universe);
	player.virtues.push_back(new center_of_attention(universe));

	// Prepare the GUI

	std::vector<renderable_entity*> GUI_objects;

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

	GUI_objects.push_back(&mouse_pos_info_gui);
	
	//process virtues lambda function

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
		//Invoke all GUI virtues (actions)
		for (auto obj : GUI_objects)
			process_virtues(obj);
		//Invoke all physical entities' virtues (actions) 
		for (auto enitity = universe.world.GetBodyList(); enitity; enitity = enitity->GetNext() )
			process_virtues((physical_entity*)enitity->GetUserData());
		// Apply entities' changes to the world
		process_virtues(&nature);

		universe.message_queues.check_all_lifetimes();
		universe.message_queues.delete_all_marked();

		universe.world.Step(1/60.f, 8, 3);

		window.clear(sf::Color::White);
		update_and_render_scene(window, universe.world);
		update_and_render_GUI_objects(window, GUI_objects);
		window.display();
	}
}