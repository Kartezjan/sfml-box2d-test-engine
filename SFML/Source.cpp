#include "source.h"

inline void update_and_render_GUI_objects(sf::RenderWindow& window, std::vector<renderable_entity*> GUI_objects) {
	for(auto obj : GUI_objects) {
		obj->update();
		window.draw(*obj);
	}
}

inline void update_and_render_scene(sf::RenderWindow& window, b2World& world) {
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
	sf::Texture bomb_texture;
	sf::Texture bang_texture;

	ground_texture.loadFromFile("ground.png");
	box_texture.loadFromFile("box.jpg");
	wheel_texture.loadFromFile("wheel.png");
	car_body_texture.loadFromFile("red_car.jpg");
	bomb_texture.loadFromFile("black.jpg");
	bang_texture.loadFromFile("bang.png");

	resources.textures.push_back(ground_texture);
	resources.textures.push_back(box_texture);
	resources.textures.push_back(wheel_texture);
	resources.textures.push_back(car_body_texture);
	resources.textures.push_back(bomb_texture);
	resources.textures.push_back(bang_texture);

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

	abstract_entity illusion_handler;
	illusion_handler.virtues.push_back(new shows_illusions(universe, window));

	std::vector<std::shared_ptr<physical_entity>> allocated_objects;
	setup_bullet_world(universe, allocated_objects);

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
		process_virtues(&illusion_handler);
		update_and_render_GUI_objects(window, GUI_objects);
		window.display();
	}
}