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
		msg.fixture_a = contact->GetFixtureA();
		msg.fixture_b = contact->GetFixtureB();
		contact_queue.push_back(msg);
	}
}

void destroys_upon_collision::send_message(abstract_entity* source) {
	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
	for (auto& msg : contact_queue) {
		if (msg.fixture_a->GetBody() == nullptr || msg.fixture_b->GetBody() == nullptr) {
			msg.delete_this_message = true;
			continue;
		}

		auto bodyA = static_cast<physical_entity*>(msg.fixture_a->GetBody()->GetUserData());
		auto bodyB = static_cast<physical_entity*>(msg.fixture_b->GetBody()->GetUserData());

		if (bodyA == source || bodyB == source) {
			auto& death_queue = cosmos.message_queues.get_queue<death_message>();
			auto does_exist = std::find_if(death_queue.begin(), death_queue.end(), [source](death_message& msg_in_vector) {
				return source == msg_in_vector.target;
			});
			if (does_exist == death_queue.end()) {
				death_message death_msg;
				death_msg.target = source;
				death_queue.push_back(death_msg);
				msg.delete_this_message = true;
			}
		}
	}
}

void explodes_upon_collision::send_message(abstract_entity* source) {
	auto entity = dynamic_cast<physical_entity*>(source);
	assert(entity);
	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
	for (auto& msg : contact_queue) {
		if (msg.fixture_a->GetBody() == (b2Body*)0xFDFDFDFD || msg.fixture_b->GetBody() == (b2Body*)0xFDFDFDFD) {
			msg.delete_this_message = true;
			continue;
		}

		auto bodyA = (physical_entity*)msg.fixture_a->GetBody()->GetUserData();
		auto bodyB = (physical_entity*)msg.fixture_b->GetBody()->GetUserData();

		if (bodyA == source || bodyB == source) {
			for (float i = 0; i < num_rays; ++i) {
				float angle = (i / num_rays * 360 * DEG_TO_RADf);
				b2Vec2 ray_direction(sinf(angle), cosf(angle) );
				b2Vec2 ray_end = entity->get_physical_body()->GetWorldCenter() + blast_radius * ray_direction;
				closest_ray_callback callback;
				cosmos.world.RayCast(static_cast<b2RayCastCallback*>(&callback), entity->get_physical_body()->GetWorldCenter(), ray_end);
				if (callback.m_body) {
					apply_blast_impulse(callback.m_body, entity->get_physical_body()->GetWorldCenter(), callback.contact_point, (blast_power / num_rays));
					//also produce "BANG!" image
					sf::RenderWindow useless_window;
					sf::Drawable* bang_sprite = new sf::Sprite(cosmos.resources.get_texture("bang"));

					dynamic_cast<sf::Sprite*>(bang_sprite)->setOrigin(sf::Vector2f(320.f, 220.f));
					dynamic_cast<sf::Sprite*>(bang_sprite)->setScale(sf::Vector2f(0.2f, 0.2f));
					dynamic_cast<sf::Sprite*>(bang_sprite)->setPosition(sf::Vector2f(entity->get_physical_body()->GetPosition().x * SCALE, entity->get_physical_body()->GetPosition().y * SCALE) );

					image_entity* bang = new image_entity(bang_sprite, "BANG!", image_entity::ILLUSION, useless_window);
					auto& illu_queue = cosmos.message_queues.get_queue<show_illusion_message>();
					show_illusion_message msg;
					msg.illusion_entity = bang;
					msg.duration = 250;
					illu_queue.push_back(msg);
				}
				msg.delete_this_message = true;
			}
		}
	}
}