#include "GUI.h"

void changes_GUI_text::process() {
	if (source->get_type() == entity_type::IMAGE) {
		image_entity* text_holder = (image_entity*)source;

		if (text_holder->get_image_type() == text_holder->TEXT) {
			std::string output = "mouse pos - X:" +
				std::to_string(cosmos_.mouse_pos.x) + " Y:" std::to_string(cosmos_.mouse_pos.y);
			auto text = dynamic_cast<sf::Text*>(text_holder->get_visual_object());
			text->setString(output);
		}
	}
	else
		printf("This virtue is only for images.\n");
}

void shows_illusions::process() {
	//remove outdated illusion
	auto current_time = cosmos_.universe_clock.getElapsedTime().asMilliseconds();
	illusions.erase(std::remove_if(illusions.begin(),
		illusions.end(),
		[current_time](const illusion_data& illusion) {return current_time > illusion.time_offset + illusion.duration; }
	), illusions.end()
	);

	//draw all remaining illusions
	for (auto current = illusions.begin(); current !=  illusions.end(); ++current)
		window.draw(*current->illusion_entity);
	//add new, if any
	auto& illu_queue = cosmos_.message_queues.get_queue<show_illusion_message>();
	for (auto& msg : illu_queue) {
		msg.delete_this_message = true;
		illusions.push_back(illusion_data{ msg.illusion_entity, current_time, msg.duration });
	}
}