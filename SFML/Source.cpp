#include "source.h"

void update_and_render_gui_objects(sf::RenderWindow& window, std::vector<renderable_entity*> gui_objects) {
	for(auto obj : gui_objects) {
		obj->update();
		window.draw(*obj);
	}
}

void update_and_render_scene(sf::RenderWindow& window, b2World& world) {
	for (auto current = world.GetBodyList(); current; current = current->GetNext() ) {
		auto current_entity = static_cast<renderable_entity*>(current->GetUserData());
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
	resources.textures_ += std::make_pair<>("ground", R"(gfx\ground.png)");
	resources.textures_ += std::make_pair<>("box", R"(gfx\box.jpg)");
	resources.textures_ += std::make_pair<>("wheel", R"(gfx\wheel.png)");
	resources.textures_ += std::make_pair<>("red_car", R"(gfx\red_car.jpg)");
	resources.textures_ += std::make_pair<>("black", R"(gfx\black.png)");
	resources.textures_ += std::make_pair<>("bang", R"(gfx\bang.png)");


	// Prepare the world
	universe universe(b2Vec2(0.0f, 9.8f), resources);

	abstract_entity user_input;
	user_input.virtues.push_back(std::make_unique<produces_user_input>(universe, window));

	abstract_entity camera;
	camera.virtues.push_back(std::make_unique<controlls_view>(50, universe, view, window));
	camera.virtues.push_back(std::make_unique<tracks_object>(universe, view, window));

	abstract_entity contact_handler;
	contact_handler.virtues.push_back(std::make_unique<handles_contacts>(universe));

	abstract_entity nature;
	nature.virtues.push_back(std::make_unique<applies_force>(universe));
	nature.virtues.push_back(std::make_unique<spawns_objects>(universe));
	nature.virtues.push_back(std::make_unique<destroys_all_doomed_objects>(universe));

	abstract_entity illusion_handler;
	illusion_handler.virtues.push_back(std::make_unique<shows_illusions>(universe, window));

	universe.entity_map += primitive_entity(create_ground(universe.world, 400.f + 12000.f, 500.f, 10000, 200), "ground", resources.textures_["ground"]);
	universe.entity_map += primitive_entity(create_ground(universe.world, 400.f, 500.f, 10000, 200), "ground", resources.textures_["ground"]);
	universe.entity_map += primitive_entity(create_ramp(universe.world, 400.f, 300.f), "ground", resources.textures_["ground"]);


	primitive_entity player(create_player(universe.world, 370, 350), "player", universe.resources.textures_["box"]);

	primitive_entity front_wheel(create_circle(universe.world, 370, 350, 20.f, 500.f, 0.7f), "front_wheel", universe.resources.textures_["wheel"]);
	primitive_entity back_wheel(create_circle(universe.world, 370, 350, 20.f, 500.f, 0.7f), "back_wheel", universe.resources.textures_["wheel"]);
	setup_car(player, front_wheel, back_wheel, b2Vec2(-280.f,-10.f), b2Vec2(280.f,-10.f), 50000, universe);
	player.virtues.push_back(std::make_unique<center_of_attention>(universe));

	primitive_entity beam(create_box(universe.world, 370, -200, 1000, 20, b2Vec2(-100,0), 0, 30.f, 0.7f), "trebuchet_beam", universe.resources.textures_["box"]);
	primitive_entity counter_weight(create_box(universe.world, 668, -150, 70, 70, 500, 0.7f), "TREB_CW", universe.resources.textures_["box"]);
	primitive_entity grand_wheel(create_circle(universe.world, 370, 90, 84, 1.0f, 0.1f), "TREB_WHEEL", universe.resources.textures_["box"]);
	setup_trebuchet(player, beam, counter_weight, grand_wheel, b2Vec2(0, -500), b2Vec2(400,0), universe);

	// Prepare the GUI
	auto example_animation = test_animation(resources, window);
	std::vector<renderable_entity*> gui_objects;

	gui_objects.emplace_back(&example_animation);

	sf::Font font;
	font.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
	sf::Text mouse_position_info;
	mouse_position_info.setFont(font);
	mouse_position_info.setString("mouse position - X: 0  Y: 0");
	mouse_position_info.setCharacterSize(12);
	mouse_position_info.setFillColor(sf::Color::Black);
	mouse_position_info.setPosition(sf::Vector2f(0, 0));


	image_entity mouse_pos_info_gui(&mouse_position_info, "GUI_MOUSE_POS", image_entity::TEXT, window);
	mouse_pos_info_gui.virtues.push_back(std::make_unique<changes_GUI_text>(universe));

	gui_objects.push_back(&mouse_pos_info_gui);
	
	//process virtues lambda function

	auto process_virtues = [](abstract_entity& target) {
		for (auto& e : target.virtues) 
			e->send_message(&target);
	};

	while (window.isOpen())
	{
		//Get input from the user
		process_virtues(user_input);
		process_virtues(camera);
		process_virtues(contact_handler);
		//Invoke all GUI virtues (actions)
		for (auto obj : gui_objects)
			process_virtues(*obj);
		//Invoke all physical entities' virtues (actions) 
		for (auto entity = universe.world.GetBodyList(); entity; entity = entity->GetNext() )
			process_virtues(*static_cast<physical_entity*>(entity->GetUserData()));
		// Apply entities' changes to the world
		process_virtues(nature);

		universe.world.Step(1/60.f, 8, 3);

		window.clear(sf::Color::White);
		update_and_render_scene(window, universe.world);
		process_virtues(illusion_handler);
		update_and_render_gui_objects(window, gui_objects);
		window.display();

		universe.message_queues.check_all_lifetimes();
		universe.message_queues.delete_all_marked();
	}
}