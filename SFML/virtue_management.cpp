//#include "virtue_management.h"
#include "contacts.h"

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
		keyboard_events[i].delete_this_message = true;
		if (keyboard_events[i].key == input_key::W) {
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp >= cooldown) {
				previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				message.force = b2Vec2(0.0f, -1000.0f);
				message.type = force_type::APPLY_IMPULS_TO_CENTER;
				send_message(keyboard_events[i], message);
			}
		}
		else if (keyboard_events[i].key == input_key::S) {
			message.force = b2Vec2(0.0f, 700.0f);
			send_message(keyboard_events[i], message);
		}
		else if (keyboard_events[i].key == input_key::A) {
			source->get_physical_body()->SetTransform(source->get_physical_body()->GetPosition(), -90 * DEG_TO_RAD);
		}
		else if (keyboard_events[i].key == input_key::D) {
			source->get_physical_body()->SetTransform(source->get_physical_body()->GetPosition(), 90 * DEG_TO_RAD);
			send_message(keyboard_events[i], message);
		}
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
				auto box = new physical_entity(create_box(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y), "box", cosmos.resources.textures[1]);
				spawned_objects.push_back(box);
			}
		}
		if (msg.key == input_key::RMB) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_removal_timestamp >= cooldown) {
				previous_removal_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				if (!spawned_objects.empty()) {
					spawned_objects.back()->~physical_entity();
					spawned_objects.pop_back();
				}
			}
		}
		if (msg.key == input_key::B) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_creation_timestamp >= cooldown) {
				previous_creation_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				auto bomb = new physical_entity(create_circle(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y, 16, 1.f, 1.f), "bomb", cosmos.resources.textures[4]);
				bomb->virtues.push_back(std::make_unique<destroys_upon_collision>(cosmos));
				bomb->virtues.push_back(std::make_unique<explodes_upon_collision>(cosmos, 200.f, 1e+5F ));
			}
		}

	}
}

void destroys_all_doomed_objects::send_message(abstract_entity* source) {
	auto& death_queue = cosmos.message_queues.get_queue<death_message>();
	for (auto& msg : death_queue) {
		msg.delete_this_message = true;
		msg.target->~abstract_entity();
	}
}

