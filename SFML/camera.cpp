#include "camera.h"

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
		printf("%s has no physical form!\n", source->name.c_str());
}