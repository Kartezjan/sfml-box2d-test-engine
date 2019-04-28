//#include "virtue_management.h"
#include "contacts.h"

void applies_force::send_message(abstract_entity* source) {
	auto& force_events = cosmos.message_queues.get_queue<force_message>();
	for (auto& force_event : force_events) {
		auto current_entity = dynamic_cast<physical_entity*>(cosmos.all_entities[force_event.target].get());
		force_event.delete_this_message= true;
		if (!current_entity) {
			printf("You cannot apply force to an abstract entity. (It is also possible that the entity has lost his physical body)\n");
			return;
		}
		switch (force_event.type) {
		case force_type::APPLY_FORCE_TO_CENTER:
			current_entity->get_physical_body()->ApplyForceToCenter(force_event.force, true);
			break;
		case force_type::APPLY_IMPULS_TO_CENTER:
			current_entity->get_physical_body()->ApplyLinearImpulseToCenter(force_event.force, true);
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
		msg.delete_this_message = true;
		cosmos.all_entities -= msg.target;
	}
}

