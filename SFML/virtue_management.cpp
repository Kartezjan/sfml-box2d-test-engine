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

	for (size_t i = 0; i < keyboard_events.size(); ++i) {
		if (keyboard_events[i].key == input_key::W) {
			keyboard_events[i].delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp >= cooldown) {
				previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				message.force = b2Vec2(0.0f, -100.0f);
				message.type = force_type::APPLY_IMPULS_TO_CENTER;
				send_message(keyboard_events[i], message);
			}
		}
		else if (keyboard_events[i].key == input_key::S) {
			keyboard_events[i].delete_this_message = true;
			message.force = b2Vec2(0.0f, 70.0f);
			send_message(keyboard_events[i], message);
		}
		else if (keyboard_events[i].key == input_key::A) {
			keyboard_events[i].delete_this_message = true;
			message.force = b2Vec2(-70.0f, 0.0f);
			send_message(keyboard_events[i], message);
		}
		else if (keyboard_events[i].key == input_key::D) {
			keyboard_events[i].delete_this_message = true;
			message.force = b2Vec2(70.0f, 0.0f);
			send_message(keyboard_events[i], message);
		}
	}
}


void produces_user_input::send_message(abstract_entity* source) {

	//update mouse pos

	cosmos.mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));


	//check if any of the desirable keys is pressed

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
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
		message.key = input_key::SPACE;
		keyboard_events.push_back(message);
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
		message.key = input_key::UP;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		message.key = input_key::DOWN;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
		message.key = input_key::LEFT;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
		message.key = input_key::RIGHT;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
		message.key = input_key::Z;
		keyboard_events.push_back(message);
	}
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::X)) {
		message.key = input_key::X;
		keyboard_events.push_back(message);
	}
	/*!!!!!!!!!!!!!!!!!!!!!!!*/
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::F) && cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp >= 300) {
		previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
		auto& camera_events = cosmos.message_queues.get_queue<camera_message>();
		camera_message cam_msg;
		cam_msg.type = event_type::FREE_CAMERA;
		camera_events.push_back(cam_msg);
	}
	/*!!!!!!!!!!!!!!!!!!!!!!!*/
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
		if (force_events[i].source->get_physical_body() == nullptr) {
			printf("You cannot apply force to an abstract entity. (It is also possible that the entity has lost his physical body)\n");
			return;
		}
		switch (force_events[i].type) {
		case force_type::APPLY_FORCE_TO_CENTER:
			force_events[i].source->get_physical_body()->ApplyForceToCenter(force_events[i].force, false);
			break;
		case force_type::APPLY_IMPULS_TO_CENTER:
			force_events[i].source->get_physical_body()->ApplyLinearImpulseToCenter(force_events[i].force, false);
			break;
		default:
			printf("Unknown force type\n");
		}
	}
}


void spawns_objects::send_message(abstract_entity* source) {
	auto& input_messages = cosmos.message_queues.get_queue<input_message>();
	for (auto& msg : input_messages) {
		if (msg.key == input_key::LMB) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_creation_timestamp >= cooldown) {
				previous_creation_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				cosmos.physical_objects.push_back(new sprite_entity(cosmos.resources.textures[1], sf::Vector2f(16.f, 16.f), create_box(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y), "box"));
			}
		}
		if (msg.key == input_key::RMB) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_removal_timestamp >= cooldown) {
				previous_removal_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				for (int i = cosmos.physical_objects.size() - 1; i >= 0; --i)
					if (cosmos.physical_objects[i]->name == "box") {
						cosmos.death_queue.push(cosmos.physical_objects[i]);
						cosmos.physical_objects.erase(cosmos.physical_objects.begin() + i);
						break;
					}
			}
		}
	}
}

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


void controlls_view::send_message(abstract_entity* source) {
	auto& input_messages = cosmos.message_queues.get_queue<input_message>();
	for (auto& msg : input_messages) {
		if (msg.key == input_key::UP) {
			msg.delete_this_message = true;
			view.move(sf::Vector2f(0, -speed));
			window.setView(view);
		}
		if (msg.key == input_key::DOWN) {
			msg.delete_this_message = true;
			view.move(sf::Vector2f(0, speed));
			window.setView(view);
		}
		if (msg.key == input_key::LEFT) {
			msg.delete_this_message = true;
			view.move(sf::Vector2f(-speed, 0));
			window.setView(view);
		}
		if (msg.key == input_key::RIGHT) {
			msg.delete_this_message = true;
			view.move(sf::Vector2f(speed, 0));
			window.setView(view);
		}
		if (msg.key == input_key::Z || msg.key == input_key::X) {
			msg.delete_this_message = true;
			bool changed = false;
			if (msg.key == input_key::Z && current_scale <= max_scale) {
				current_scale += delta_scale;
				changed = true;
			}
			else if (current_scale >= min_scale) {
				current_scale -= delta_scale;
				changed = true;
			}
			if (changed) {
				sf::Vector2u view_size = window.getSize();
				view_size.x *= current_scale;
				view_size.y *= current_scale;
				view.setSize(sf::Vector2f(view_size.x, view_size.y));
				window.setView(view);
			}
		}
	}
}

void tracks_object::send_message(abstract_entity* source) {
	auto& camera_events = cosmos.message_queues.get_queue<camera_message>();
	for (auto& msg : camera_events) {
		if (msg.type == event_type::FREE_CAMERA) {
			msg.delete_this_message = true;
			free_mode = !free_mode;
		}
		else if (msg.type == event_type::TRACKED_OBJECT_POS) {
			msg.delete_this_message = true;
			if (!free_mode) {
				if (msg.direction == move_direction::LEFT)
					view.setCenter(sf::Vector2f(msg.event_pos.x - window.getSize().x * 3 / 8, msg.event_pos.y - window.getSize().y * 1 / 8));
				else
					view.setCenter(sf::Vector2f(msg.event_pos.x + window.getSize().x * 3 / 8, msg.event_pos.y - window.getSize().y * 1 / 8));
				window.setView(view);
			}
		}
	}
}

void center_of_attention::send_message(abstract_entity* source) {
	if (source->get_physical_body() != nullptr) {
		camera_message msg;
		msg.event_pos = sf::Vector2f(source->get_physical_body()->GetPosition().x * SCALE, source->get_physical_body()->GetPosition().y * SCALE);
		if (source->get_physical_body()->GetLinearVelocity().x < -5)
			msg.direction = move_direction::LEFT;
		else
			msg.direction = move_direction::RIGHT;
		msg.type = event_type::TRACKED_OBJECT_POS;
		auto& camera_events = cosmos.message_queues.get_queue<camera_message>();
		camera_events.push_back(msg);
	}
	else
		printf("%s has no physical form!\n", source->name);
}