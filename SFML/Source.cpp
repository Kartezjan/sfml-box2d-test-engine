#include "source.h"
#include "editor.h"
#include "processes_hp_messages.h"

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


int main()
{

	//Prepare the window 
	sf::RenderWindow window(sf::VideoMode(1024, 768, 32), "Force Boy The Movie The Game 2: Electric Boogaloo");

	window.setFramerateLimit(60);

	//Prepare the viewpoint

	sf::View view = window.getDefaultView();
	window.setView(view);

	// Prepare textures
	resource_manager resources;
	resources.textures_ += {"ground_large", R"(gfx\ground_large.png)"};
	resources.textures_["ground_large"].setRepeated(true);
resources.textures_ += {"ground_top", R"(gfx\ground_top.png)"};
	resources.textures_["ground_top"].setRepeated(true);
	resources.textures_ += {"wall1", R"(gfx\wall.png)"};
	resources.textures_["wall1"].setRepeated(true);
	resources.textures_ += {"box", R"(gfx\box.jpg)"};
	resources.textures_ += {"wheel", R"(gfx\wheel.png)"};
	resources.textures_ += {"red_car", R"(gfx\red_car.jpg)"};
	resources.textures_ += {"black", R"(gfx\black.png)"};
	resources.textures_ += {"bang", R"(gfx\bang.png)"};
	resources.textures_ += {"cyan", R"(gfx\cyan.png)"};
	resources.textures_ += {"heart", R"(gfx\heart.png)"};

	resources.font = sf::Font();
	resources.font.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");

	sf::Music theme;
	if (!theme.openFromFile(R"(sfx\DOS88CityStomper.ogg)"))
	{
		printf("Cannot open file 'DOS88CityStomper.ogg'");
		return 0;
	}
	theme.setLoop(true);
	theme.setVolume(30);
	theme.play();
		
	resources.add_sound_effect("lazer", R"(sfx\lazer.ogg)");
	resources.add_sound_effect("walk", R"(sfx\step_metal.ogg)");
	resources.add_sound_effect("force", R"(sfx\UnrelentingForce.ogg)");
	resources.add_sound_effect("ball", R"(sfx\IceForm.ogg)");

	test_animation(resources, window);


	// Prepare the world
	universe universe(b2Vec2(0.0f, 9.8f), resources, window);

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
	nature.virtues.emplace_back(std::make_unique<processes_hp_messages>(universe));
	//nature.virtues.emplace_back(std::make_unique<editor>(universe, window));

	abstract_entity editor_entity;
	editor_entity.virtues.emplace_back(std::make_unique<editor>(universe, window));

	abstract_entity illusion_handler;
	illusion_handler.virtues.push_back(std::make_unique<shows_illusions>(universe, window));

	entity_id ground = 0;
	universe.all_entities += new primitive_entity(create_ground(universe.world, 400.f + 12000.f, 700.f, 10000, 600), "ground", resources.textures_["ground_large"]);
	ground = universe.all_entities += new primitive_entity(create_ground(universe.world, 400.f, 700.f, 10000, 582), "ground", resources.textures_["ground_large"]);
	dynamic_cast<primitive_entity*>(universe.all_entities[ground].get())->expand_texture_rect();
	ground = universe.all_entities += new primitive_entity(create_ground(universe.world, 400.f, 400.f, 10000, 18), "ground", resources.textures_["ground_top"]);
	dynamic_cast<primitive_entity*>(universe.all_entities[ground].get())->expand_texture_rect();
	universe.all_entities += new primitive_entity(create_ground(universe.world, 400.f, 700.f, 10000, 600), "ground", resources.textures_["ground_large"]);
	universe.all_entities += new primitive_entity(create_ramp(universe.world, 400.f, 300.f), "ground", resources.textures_["ground_large"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 2000.f, 300.f,100.f, 750.f), "wall1", resources.textures_["wall1"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 3800.f, 0.f,100.f, 500.f), "wall2", resources.textures_["wall1"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 4800.f, 100.f,100.f, 750.f), "wall3", resources.textures_["wall1"]);

	universe.all_entities += new primitive_entity(create_ground(universe.world, 3875.f, -250.f, 150, 50), "ground", resources.textures_["ground_large"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 4725.f, -250.f, 150, 50), "ground", resources.textures_["ground_large"]);

	universe.all_entities += new primitive_entity(create_ground(universe.world, 3875.f, -950.f, 150, 50), "ground", resources.textures_["ground_large"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 4725.f, -950.f, 150, 50), "ground", resources.textures_["ground_large"]);

	universe.all_entities += new primitive_entity(create_ground(universe.world, 3875.f, -650.f, 150, 50), "ground", resources.textures_["ground_large"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 4725.f, -650.f, 150, 50), "ground", resources.textures_["ground_large"]);

	universe.all_entities += new primitive_entity(create_ground(universe.world, 3800.f, -750.f, 100.f, 1000.f), "wall2", resources.textures_["wall1"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 4800.f, -750.f, 100.f, 1000.f), "wall3", resources.textures_["wall1"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 4100.f, -1250.f, 800, 10), "ground", resources.textures_["ground_large"]);

	universe.all_entities += new primitive_entity(create_ground(universe.world, 5726.f, 154.f, 200, 150), "ground", resources.textures_["ground_large"]);
	universe.all_entities += new primitive_entity(create_ground(universe.world, 6160.f, -132.f, 200, 150), "ground", resources.textures_["ground_large"]);
	make_obstacles(universe);

	trebuchet_spawn(universe);
	auto hero_handle = hero_test(universe);

	// Prepare the GUI
	sf::Font font;
	font.loadFromFile(R"(C:\Windows\Fonts\arial.ttf)");
	auto mouse_position_info = new sf::Text;
	mouse_position_info->setFont(font);
	mouse_position_info->setString("mouse position - X: 0  Y: 0");
	mouse_position_info->setCharacterSize(12);
	mouse_position_info->setFillColor(sf::Color::Black);
	mouse_position_info->setPosition(sf::Vector2f(0, 0));

	auto mouse_text_handle = universe.gui_resources += new image_entity (mouse_position_info, { 0,0 }, "GUI_MOUSE_POS", image_entity::MOUSE_CORD_MESSAGE, window);
	auto mouse_text = dynamic_cast<image_entity*>(universe.gui_resources[mouse_text_handle].get());
	mouse_text->sticky(true);
	mouse_text->virtues.push_back(std::make_unique<changes_GUI_text>(universe));

	auto health_status = new sf::Text("100", font, 40);
	health_status->setFillColor(sf::Color(220, 0, 40));
	auto heart_img = new sf::RectangleShape(sf::Vector2f{45,45});
	heart_img->setTexture(&universe.resources.textures_["heart"]);
	auto heart_handle = universe.gui_resources += new image_entity(heart_img, { 0,20 }, "HEART", image_entity::ILLUSION, window);

	auto hp_status_handle = universe.gui_resources += new image_entity(health_status, { 50, 15 }, "HP_STATUS", image_entity::TEXT, window);
	auto hp_status = dynamic_cast<image_entity*>(universe.gui_resources[hp_status_handle].get());
	hp_status->sticky(true);
	auto heart = dynamic_cast<image_entity*>(universe.gui_resources[heart_handle].get());
	hp_status->virtues.emplace_back(std::make_unique<updates_hit_points_status>(universe, hero_handle));
	heart->sticky(true);

	
	//process virtues lambda function

	auto process_virtues = [](abstract_entity& target) {
		for (auto& e : target.virtues) 
			e->send_message(&target);
	};

	sf::Event ev;
	while (window.isOpen())
	{
		while (window.pollEvent(ev))
		{
			if(ev.type == sf::Event::Closed)
			{
				return(0);
			}

		}
		//Get input from the user
		process_virtues(user_input);
		process_virtues(camera);
		process_virtues(contact_handler);
		//Invoke all physical entities' virtues (actions) 
		process_virtues(editor_entity);
		for(auto& gui_object : universe.gui_resources)
		{
			process_virtues(*gui_object.second);
		}
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