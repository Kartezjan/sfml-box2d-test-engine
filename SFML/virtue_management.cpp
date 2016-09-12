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
				//box->virtues.push_back(new destroys_upon_collision(cosmos));
				cosmos.physical_objects.push_back(box);
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

void adds_to_death_queue::send_message(abstract_entity* source) {
	auto& death_queue = cosmos.message_queues.get_queue<death_message>();
	for (auto& msg : death_queue) {
		msg.delete_this_message = true;
		cosmos.death_queue.push(msg.target);
		cosmos.physical_objects.erase(std::remove_if(cosmos.physical_objects.begin(), cosmos.physical_objects.end(), [msg](const abstract_entity* entity) {return entity == msg.target;} ) );
	}
}