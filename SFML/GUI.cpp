#include "GUI.h"
#include "sprite_entity.h"

void setup_gui(std::vector<renderable_entity*>& container, sf::RenderWindow& win_ref, universe& uni_ref)
{

}
void changes_GUI_text::send_message(abstract_entity* source) {
	if (source->get_type() == entity_type::IMAGE) {

		auto text_holder = dynamic_cast<image_entity*>(source);
		switch(text_holder->get_image_type())
		{
		case image_entity::MOUSE_CORD_MESSAGE:
			{
				std::string output = "mouse pos - X:" + std::to_string(cosmos.mouse_pos.x)
					+ " Y:" + std::to_string(cosmos.mouse_pos.y);
				sf::Text* text = (sf::Text*)text_holder->get_visual_object();
				text->setString(output);
			}
		}
	}
	else
		printf("This virtue is only for images.\n");
}

void updates_hit_points_status::send_message(abstract_entity* source)
{
	auto text_holder = dynamic_cast<image_entity*>(source);
	assert(text_holder);
	auto text = dynamic_cast<sf::Text*>(text_holder->get_visual_object());
	const auto hp = dynamic_cast<sprite_entity*>(cosmos.all_entities[target_].get())->hp();
	text->setString(hp <= 0 ? "0" : std::to_string(hp));
}

void shows_illusions::send_message(abstract_entity* source) {
	//remove outdated illusion
	auto current_time = cosmos.universe_clock.getElapsedTime().asMilliseconds();
	illusions.erase(std::remove_if(illusions.begin(),
		illusions.end(),
		[current_time](const illusion_data& illusion) {return current_time > illusion.time_offset + illusion.duration; }
	), illusions.end()
	);

	//draw all remaining illusions
	for (auto current = illusions.begin(); current !=  illusions.end(); ++current)
		window.draw(*current->illusion_entity);
	//add new, if any
	auto& illu_queue = cosmos.message_queues.get_queue<show_illusion_message>();
	for (auto& msg : illu_queue) {
		msg.delete_this_message = true;
		illusions.push_back(illusion_data{std::unique_ptr<image_entity>(msg.illusion_entity), current_time, msg.duration });
	}
}
