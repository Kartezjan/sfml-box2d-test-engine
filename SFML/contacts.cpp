#include "contacts.h"

void handles_contacts::send_message(abstract_entity* source) {
	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
	for (auto contact = cosmos.world.GetContactList(); contact; contact = contact->GetNext()) {
		contact_message msg;
		msg.lifetime = 2;
		if (!contact->IsEnabled())
			return;
		if (contact->IsTouching())
			msg.contact_type = contact_type::COLLISION;
		else
			msg.contact_type = contact_type::AABB_CONTACT;
		msg.fixtureA = contact->GetFixtureA();
		msg.fixtureB = contact->GetFixtureB();
		contact_queue.push_back(msg);
	}
}

void destroys_upon_collision::send_message(abstract_entity* source) {
	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
	for (auto& msg : contact_queue) {
		if (msg.fixtureA->GetBody() == (b2Body*)0xFDFDFDFD || msg.fixtureB->GetBody() == (b2Body*)0xFDFDFDFD || msg.fixtureA->GetBody()->GetType() == b2_staticBody || msg.fixtureB->GetBody()->GetType() == b2_staticBody) {
			msg.delete_this_message = true;
			return;
		}

		auto bodyA = (physical_entity*)msg.fixtureA->GetBody()->GetUserData();
		auto bodyB = (physical_entity*)msg.fixtureB->GetBody()->GetUserData();
		//if(bodyA == source)
		//	printf("%s <--> %s\n", bodyA->name.c_str(), bodyB->name.c_str());
		if (bodyA == source || bodyB == source) {
			msg.delete_this_message = true;
			auto& death_queue = cosmos.message_queues.get_queue<death_message>();
			auto does_exist = std::find_if(death_queue.begin(), death_queue.end(), [source](death_message& msg_in_vector) {
				return source == msg_in_vector.target;
			});
			if (does_exist == death_queue.end()) {
				death_message death_msg;
				death_msg.target = source;
				death_queue.push_back(death_msg);
			}
		}
	}
}