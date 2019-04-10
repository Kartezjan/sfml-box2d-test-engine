#include "GUI.h"

void changes_GUI_text::send_message(abstract_entity* source) {
	if (source->get_type() == entity_type::IMAGE) {

		image_entity* text_holder = (image_entity*)source;

		if (text_holder->get_image_type() == text_holder->TEXT) {
			char buffer[100];
			std::string output = "mouse pos - X:";
			_itoa_s(cosmos.mouse_pos.x, buffer, 10);
			output.append(buffer);
			output.append("  Y:");
			_itoa_s(cosmos.mouse_pos.y, buffer, 10);
			output.append(buffer);

			sf::Text* text = (sf::Text*)text_holder->get_visual_object();
			text->setString(output);
		}
	}
	else
		printf("This virtue is only for images.\n");
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
		illusions.push_back(illusion_data{ msg.illusion_entity, current_time, msg.duration });
	}
}