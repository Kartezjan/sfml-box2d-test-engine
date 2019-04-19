#include "camera.h"
#include "entity.h"
#include "world.h"

void controls_view::process() {
	auto& input_messages = cosmos_.message_queues.get_queue<input_message>();
	for (auto& msg : input_messages) {
		if (msg.key == input_key::UP) {
			msg.delete_this_message = true;
			view_.move(sf::Vector2f(0, -speed_));
			window_ref_.setView(view_);
		}
		else if (msg.key == input_key::DOWN) {
			msg.delete_this_message = true;
			view_.move(sf::Vector2f(0, speed_));
			window_ref_.setView(view_);
		}
		else if (msg.key == input_key::LEFT) {
			msg.delete_this_message = true;
			view_.move(sf::Vector2f(-speed_, 0));
			window_ref_.setView(view_);
		}
		else if (msg.key == input_key::RIGHT) {
			msg.delete_this_message = true;
			view_.move(sf::Vector2f(speed_, 0));
			window_ref_.setView(view_);
		}
		else if (msg.key == input_key::Z || msg.key == input_key::X) {
			 msg.delete_this_message = true;
			bool changed = false;
			if (msg.key == input_key::Z && current_scale_ <= max_scale_) {
				current_scale_ += delta_scale_;
				changed = true;
			}
			else if (current_scale_ >= min_scale_) {
				current_scale_ -= delta_scale_;
				changed = true;
			}
			if (changed) {
				sf::Vector2u view_size = window_ref_.getSize();
				view_size.x *= current_scale_;
				view_size.y *= current_scale_;
				view_.setSize(sf::Vector2f(view_size.x, view_size.y));
				window_ref_.setView(view_);
			}
		}
	}
}

void tracks_object::process() {
	auto& camera_events = cosmos_.message_queues.get_queue<camera_message>();
	for (auto& msg : camera_events) {
		if (msg.type == event_type::FREE_CAMERA) {
			msg.delete_this_message = true;
			free_mode_ = !free_mode_;
		}
		else if (msg.type == event_type::TRACKED_OBJECT_POS) {
			msg.delete_this_message = true;
			if (!free_mode_) {
				if (msg.direction == move_direction::LEFT)
					view_.setCenter(sf::Vector2f(msg.event_pos.x - window_ref_.getSize().x * 3 / 8, msg.event_pos.y - window_ref_.getSize().y * 1 / 8));
				else
					view_.setCenter(sf::Vector2f(msg.event_pos.x + window_ref_.getSize().x * 3 / 8, msg.event_pos.y - window_ref_.getSize().y * 1 / 8));
				window_ref_.setView(view_);
			}
		}
	}
}

void center_of_attention::process() {
	const auto body_virtue = dynamic_cast<has_physics*>(owner_ref_.get_virtue(virtue_type::has_physics).get());
	if (body_virtue) {
		const auto body = body_virtue->physical_body();
		camera_message msg;
		msg.event_pos = sf::Vector2f(body->GetPosition().x * SCALE, body->GetPosition().y * SCALE);
		if (body->GetLinearVelocity().x < -5)
			msg.direction = move_direction::LEFT;
		else
			msg.direction = move_direction::RIGHT;
		msg.type = event_type::TRACKED_OBJECT_POS;
		auto& camera_events = cosmos_.message_queues.get_queue<camera_message>();
		camera_events.push_back(msg);
	}
	else
		printf("#%d has no physical form!\n", owner_ref_.id());
}