//#include "virtue_management.h"
#include "contacts.h"

void controllable::send_message(abstract_entity* source) {
	auto entity = dynamic_cast<physical_entity*>(source);
	assert(entity);
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
			entity->get_physical_body()->SetTransform(entity->get_physical_body()->GetPosition(), -90 * DEG_TO_RADf);
		}
		else if (keyboard_events[i].key == input_key::D) {
			entity->get_physical_body()->SetTransform(entity->get_physical_body()->GetPosition(), 90 * DEG_TO_RADf);
			send_message(keyboard_events[i], message);
		}
	}
}

void applies_force::send_message(abstract_entity* source) {
	auto& force_events = cosmos.message_queues.get_queue<force_message>();
	for (auto& force_event : force_events) {
		auto current_entity = dynamic_cast<physical_entity*>(force_event.source);
		force_event.delete_this_message= true;
		if (!current_entity) {
			printf("You cannot apply force to an abstract entity. (It is also possible that the entity has lost his physical body)\n");
			return;
		}
		switch (force_event.type) {
		case force_type::APPLY_FORCE_TO_CENTER:
			current_entity->get_physical_body()->ApplyForceToCenter(force_event.force, false);
			break;
		case force_type::APPLY_IMPULS_TO_CENTER:
			current_entity->get_physical_body()->ApplyLinearImpulseToCenter(force_event.force, false);
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
				auto handle = cosmos.all_entities += new primitive_entity(
					create_box(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y), "box", cosmos.resources.textures_["box"]
				);
				spawned_objects.push_back(handle);
			}
		}
		if (msg.key == input_key::RMB) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_removal_timestamp >= cooldown) {
				previous_removal_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				if (!spawned_objects.empty()) {
					const entity_id obj_to_del_handle = spawned_objects.back();
					auto& death_queue = cosmos.message_queues.get_queue<death_message>();
					death_message msg;
					msg.target = obj_to_del_handle;
					death_queue.emplace_back(msg);
					spawned_objects.pop_back();
				}
			}
		}
		if (msg.key == input_key::B) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_creation_timestamp >= cooldown) {
				previous_creation_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				auto handle = cosmos.all_entities += new primitive_entity(
					create_circle(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y, 16, 1.f, 1.f), "bomb", cosmos.resources.textures_["black"]
				);
				spawned_objects.push_back(handle);
				auto& obj = *cosmos.all_entities.access(handle).get();
				obj.virtues.push_back(std::make_unique<destroys_upon_collision>(cosmos));
				obj.virtues.push_back(std::make_unique<explodes_upon_collision>(cosmos, 200.f, 1e+5F ));
			}
		}

	}
}

void destroys_all_doomed_objects::send_message(abstract_entity* source) {
	using namespace std::placeholders;
	auto& death_queue = cosmos.message_queues.get_queue<death_message>();
	for (auto& msg : death_queue) {
		if(auto entity = dynamic_cast<physical_entity*>(cosmos.all_entities[msg.target].get())) {
			auto body = entity->get_physical_body();
			auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
			std::for_each(contact_queue.begin(), contact_queue.end(), std::bind(discard_all_messages, body, _1));
		}
		msg.delete_this_message = true;
		cosmos.all_entities -= msg.target;
	}
}

