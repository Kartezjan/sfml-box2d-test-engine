#include "virtue_management.h"

void controllable::send_message(abstract_entity* source) {
	auto& keyboard_events = cosmos.message_queues.get_queue<input_message>();
	auto& force_events = cosmos.message_queues.get_queue<force_message>();
	force_message message;
	message.source = source;
	message.type = force_type::APPLY_FORCE_TO_CENTER;

	auto send_message = [&force_events](input_message& msg_to_be_marked, force_message& new_msg) {
		force_events.push_back(new_msg);
		msg_to_be_marked.delete_this_message = true;
	};

	for (int i = 0; i < keyboard_events.size(); ++i) {
		if (keyboard_events[i].key == input_key::W && cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp >= cooldown) {
			previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
			message.force = b2Vec2(0.0f, -10.0f);
			message.type = force_type::APPLY_IMPULS_TO_CENTER;
			send_message(keyboard_events[i], message);
		}
		else if (keyboard_events[i].key == input_key::S) {
			message.force = b2Vec2(0.0f, 18.0f);
			send_message(keyboard_events[i], message);
		}
		else if (keyboard_events[i].key == input_key::A) {
			message.force = b2Vec2(-18.0f, 0.0f);
			send_message(keyboard_events[i], message);
		}
		else if (keyboard_events[i].key == input_key::D) {
			message.force = b2Vec2(18.0f, 0.0f);
			send_message(keyboard_events[i], message);
		}
	}
}


void produces_user_input::send_message(abstract_entity* source) {

	auto& keyboard_events = cosmos.message_queues.get_queue<input_message>();
	input_message message;
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
		message.key = input_key::W;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
		message.key = input_key::S;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
		message.key = input_key::A;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
		message.key = input_key::D;
		keyboard_events.push_back(message);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
		message.key = input_key::LMB;
		keyboard_events.push_back(message);
	}
	if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
		message.key = input_key::RMB;
		keyboard_events.push_back(message);
	}
}

void applies_force::send_message(abstract_entity* source) {
	auto& force_events = cosmos.message_queues.get_queue<force_message>();
	for (size_t i = 0; i < force_events.size(); ++i) {
		force_events[i].delete_this_message = true;
		if (force_events[i].source->physical_body == nullptr) {
			printf("You cannot apply force to an abstract entity. (It is also possible that the entity has lost his physical body)\n");
			return;
		}
		switch (force_events[i].type) {
		case force_type::APPLY_FORCE_TO_CENTER:
			force_events[i].source->physical_body->ApplyForceToCenter(force_events[i].force, false);
			break;
		case force_type::APPLY_IMPULS_TO_CENTER:
			force_events[i].source->physical_body->ApplyLinearImpulseToCenter(force_events[i].force, false);
			break;
		default:
			printf("Unknown force type\n");
		}
	}
}


void spawns_objects::send_message(abstract_entity* source) {
	auto& input_messages = cosmos.message_queues.get_queue<input_message>();
	for (auto msg : input_messages) {
		if (msg.key == input_key::LMB && cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_creation_timestamp >= cooldown) {
			previous_creation_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
			msg.delete_this_message = true;
			cosmos.physical_objects.push_back(new sprite_entity(cosmos.resources.textures[1], sf::Vector2f(16.f, 16.f), create_box(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y), "box"));

		}
		if (msg.key == input_key::RMB && cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_removal_timestamp >= cooldown) {
			previous_removal_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
			msg.delete_this_message = true;
			for (size_t i = cosmos.physical_objects.size() - 1; i >= 0; --i)
				if (cosmos.physical_objects[i]->name == "box") {
					cosmos.death_queue.push(cosmos.physical_objects[i]);
					cosmos.physical_objects.erase(cosmos.physical_objects.begin() + i);
					break;
				}
		}
	}
}

