#include "source.h"
#include "editor.h"

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

	resources.font = sf::Font();
	resources.font.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");

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
	// nature.virtues.push_back(std::make_unique<spawns_objects>(universe));
	nature.virtues.push_back(std::make_unique<destroys_all_doomed_objects>(universe));
	//nature.virtues.emplace_back(std::make_unique<editor>(universe, window));

	abstract_entity editor_entity;
	editor_entity.virtues.emplace_back(std::make_unique<editor>(universe, window));

	abstract_entity illusion_handler;
	illusion_handler.virtues.push_back(std::make_unique<shows_illusions>(universe, window));

	universe.all_entities += new primitive_entity(create_ground(universe.world, 400.f + 12000.f, 500.f, 10000, 200), "ground", resources.textures_["ground"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 400.f, 500.f, 10000, 200), "ground", resources.textures_["ground"]);
	universe.all_entities += new primitive_entity(create_ramp(universe.world, 400.f, 300.f), "ground", resources.textures_["ground"]);

	trebuchet_spawn(universe);

	// Prepare the GUI
	auto example_animation = test_animation(resources, window);
	std::vector<renderable_entity*> gui_objects;

	sf::Font font;
	font.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
	sf::Text mouse_position_info;
	mouse_position_info.setFont(font);
	mouse_position_info.setString("mouse position - X: 0  Y: 0");
	mouse_position_info.setCharacterSize(12);
	mouse_position_info.setFillColor(sf::Color::Black);
	mouse_position_info.setPosition(sf::Vector2f(0, 0));

	image_entity mouse_pos_info_gui(&mouse_position_info, { 0,0 }, "GUI_MOUSE_POS", image_entity::MOUSE_CORD_MESSAGE, window);
	mouse_pos_info_gui.sticky(true);
	mouse_pos_info_gui.virtues.push_back(std::make_unique<changes_GUI_text>(universe));

	gui_objects.push_back(&mouse_pos_info_gui);
	gui_objects.emplace_back(&example_animation);

	hero_test(universe);
	
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
		process_virtues(editor_entity);
		if (!universe.editor_mode)
		{
			for (auto entity = universe.world.GetBodyList(); entity; entity = entity->GetNext())
				process_virtues(*static_cast<physical_entity*>(entity->GetUserData()));
			// Apply entities' changes to the world
			process_virtues(nature);

			if (!universe.editor_mode) {
				universe.world.Step(1 / 60.f, 8, 3);
			}
		}
		window.clear(sf::Color::White);
		update_and_render_scene(window, universe.world);
		process_virtues(illusion_handler);
		update_and_render_gui_objects(window, gui_objects);
		for (auto& obj : universe.gui_resources) {
			const auto renderable = dynamic_cast<renderable_entity*>(obj.second.get());
			renderable->update();
			window.draw(*renderable);
		}
		window.display();

		universe.message_queues.check_all_lifetimes();
		universe.message_queues.delete_all_marked();
	}
}