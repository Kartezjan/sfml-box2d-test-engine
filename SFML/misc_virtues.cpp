#include "misc_virtues.h"

#include "virtue.h"
#include "contacts.h"
#include "has_physics.h"

void controllable::process() {
	auto body_virtue = dynamic_cast<has_physics>(owner_ref_.get_virtue(virtue_type::has_physics).get());
	assert(body_virtue);
	auto body = body_virtue->physical_body();
	auto& keyboard_events = cosmos_.message_queues.get_queue<input_message>();
	auto& force_events = cosmos_.message_queues.get_queue<force_message>();
	force_message message;
	message.source = owner_ref_.id();
	message.type = force_type::APPLY_FORCE_TO_CENTER;

	auto send_message = [&force_events](input_message& msg_to_be_marked, force_message& new_msg) {
		force_events.push_back(new_msg);
		msg_to_be_marked.delete_this_message = true;
	};

	for (size_t i = 0; i < keyboard_events.size(); ++i) {
		keyboard_events[i].delete_this_message = true;
		if (keyboard_events[i].key == input_key::W) {
			if (cosmos_.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp >= cooldown) {
				previous_timestamp = cosmos_.universe_clock.getElapsedTime().asMilliseconds();
				message.force = b2Vec2(0.0f, -1000.0f);
				message.type = force_type::APPLY_IMPULS_TO_CENTER;
				process();
			}
		}
		else if (keyboard_events[i].key == input_key::S) {
			message.force = b2Vec2(0.0f, 700.0f);
			process();
		}
		else if (keyboard_events[i].key == input_key::A) {
			body->SetTransform(body->GetPosition(), -90 * DEG_TO_RADf);
		}
		else if (keyboard_events[i].key == input_key::D) {
			body()->SetTransform(body->GetPosition(), 90 * DEG_TO_RADf);
			process();
		}
	}
}

void applies_force::process() {
	auto& force_events = cosmos_.message_queues.get_queue<force_message>();
	for (auto& force_event : force_events) {
		auto current_entity = force_event.source;
		auto body_virtue = dynamic_cast<has_physics>(owner_ref_.get_virtue(virtue_type::has_physics).get());
		assert(body_virtue, "you cannot apply force to non physical entity");
		auto body = body_virtue->physical_body();
		force_event.delete_this_message= true;
		
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


void spawns_objects::process() {
	auto& input_messages = cosmos_.message_queues.get_queue<input_message>();
	for (auto& msg : input_messages) {
		if (msg.key == input_key::LMB) {
			msg.delete_this_message = true;
			if (cosmos_.universe_clock.getElapsedTime().asMilliseconds() - previous_creation_timestamp >= cooldown) {
				previous_creation_timestamp = cosmos_.universe_clock.getElapsedTime().asMilliseconds();
				spawned_objects.push_back(
					std::make_unique<primitive_entity>(
						create_box(cosmos_.world, cosmos_.mouse_pos.x, cosmos_.mouse_pos.y), "box", cosmos_.resources.textures_["box"]
					)
				);
			}
		}
		if (msg.key == input_key::RMB) {
			msg.delete_this_message = true;
			if (cosmos_.universe_clock.getElapsedTime().asMilliseconds() - previous_removal_timestamp >= cooldown) {
				previous_removal_timestamp = cosmos_.universe_clock.getElapsedTime().asMilliseconds();
				if (!spawned_objects.empty()) {
					using namespace std::placeholders;
					auto& obj_to_del = spawned_objects.back();
					auto& contact_queue = cosmos_.message_queues.get_queue<contact_message>();
					std::for_each(contact_queue.begin(), contact_queue.end(), std::bind(discard_all_messages, obj_to_del->get_physical_body(), _1));
					spawned_objects.pop_back();
				}
			}
		}
		if (msg.key == input_key::B) {
			msg.delete_this_message = true;
			if (cosmos_.universe_clock.getElapsedTime().asMilliseconds() - previous_creation_timestamp >= cooldown) {
				previous_creation_timestamp = cosmos_.universe_clock.getElapsedTime().asMilliseconds();
				spawned_objects.push_back(
					std::make_unique<primitive_entity>(
						create_circle(cosmos_.world, cosmos_.mouse_pos.x, cosmos_.mouse_pos.y, 16, 1.f, 1.f), "bomb", cosmos_.resources.textures_["bomb"]
					)
				);
				spawned_objects.back()->virtues.push_back(std::make_unique<destroys_upon_collision>(cosmos_));
				spawned_objects.back()->virtues.push_back(std::make_unique<explodes_upon_collision>(cosmos_, 200.f, 1e+5F ));
			}
		}

	}
}

void destroys_all_doomed_objects::process() {
	using namespace std::placeholders;
	auto& death_queue = cosmos_.message_queues.get_queue<death_message>();
	for (auto& msg : death_queue) {
		if(auto entity = dynamic_cast<physical_entity*>(msg.target)) {
			auto body = entity->get_physical_body();
			auto& contact_queue = cosmos_.message_queues.get_queue<contact_message>();
			std::for_each(contact_queue.begin(), contact_queue.end(), std::bind(discard_all_messages, body, _1));
		}
		msg.delete_this_message = true;
		msg.target->~abstract_entity();
	}
}

