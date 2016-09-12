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