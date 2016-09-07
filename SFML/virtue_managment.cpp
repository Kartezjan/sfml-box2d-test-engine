#include "virtue_managment.h"

void controllable::send_message(abstract_entity* source, complete_message_storage& message_queue) {
	auto& keyboard_events = message_queue.get_queue<keyboard_input_message>();
	auto& force_events = message_queue.get_queue<force_message>();
	force_message message;
	message.source = source;
	message.type = force_type::APPLY_TO_CENTER;

	auto send_message = [](keyboard_input_message& msg_to_be_marked, force_message& new_msg, std::vector<force_message>& force_events) {
		force_events.push_back(new_msg);
		msg_to_be_marked.delete_this_message = true;
	};

	for (int i = 0; i < keyboard_events.size(); ++i) {
		if (keyboard_events[i].key == "W") {
			message.force = b2Vec2(0.0f, -30.0f);
			send_message(keyboard_events[i], message, force_events);
		}
		else if (keyboard_events[i].key == "S") {
			message.force = b2Vec2(0.0f, 30.0f);
			send_message(keyboard_events[i], message, force_events);
		}
		else if (keyboard_events[i].key == "A") {
			message.force = b2Vec2(-30.0f, 0.0f);
			send_message(keyboard_events[i], message, force_events);
		}
		else if (keyboard_events[i].key == "D") {
			message.force = b2Vec2(30.0f, 0.0f);
			send_message(keyboard_events[i], message, force_events);
		}
	}
}


void produces_user_input::send_message(abstract_entity* source, complete_message_storage& message_queue) {
	auto& keyboard_events = message_queue.get_queue<keyboard_input_message>();
	keyboard_input_message message;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		message.key = "W";
		keyboard_events.push_back(message);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		message.key = "S";
		keyboard_events.push_back(message);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		message.key = "A";
		keyboard_events.push_back(message);
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		message.key = "D";
		keyboard_events.push_back(message);
	}
}

void applies_force::send_message(abstract_entity* source, complete_message_storage& message_queue) {
	auto& force_events = message_queue.get_queue<force_message>();
	for (size_t i = 0; i < force_events.size(); ++i) {
		force_events[i].delete_this_message = true;
		switch (force_events[i].type) {
		case force_type::APPLY_TO_CENTER:
			if (force_events[i].source->physical_body == nullptr)
				printf("You cannot apply force to an abstract entity. (It is also possible that the entity has lost his physical body)\n");
			else {
				force_events[i].source->physical_body->ApplyForceToCenter(force_events[i].force, false);
			}
			break;
		default:
			printf("Unknown force type\n");
		}
	}
}
