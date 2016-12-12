#include "contacts.h"

#define TEST_SCALE_DOWN 1.f

inline float calculate_angle(b2Vec2 a, b2Vec2 b) {
	if (b.x == a.x)
		return b2_pi / 2;
	return atan((b.y - a.y) / (b.x - a.x));
}

inline std::pair<float, float> calculate_linear_function_params(b2Vec2 a, b2Vec2 b) {
	return std::make_pair<float, float>((b.y - a.y) / (b.x - a.x), a.y - (b.y - a.y) / (b.x - a.x) * a.x);
}

inline bool minimum_angle_test(b2Fixture* destructable_fixture, b2Vec2 contact_point,float angle) {
	auto shape = static_cast<b2PolygonShape*>(destructable_fixture->GetShape());
	for (int i = 0; i < shape->GetVertexCount(); ++i) {
		const auto pointA = shape->GetVertex(i);
		const auto pointB = shape->GetVertex((i + 1) % shape->GetVertexCount());
		float side_angle = calculate_angle(pointA, pointB);
		if (side_angle < 0)
			side_angle = 2 * b2_pi + side_angle;
		if (angle < 0)
			angle = 2 * b2_pi + angle;
		while (angle > 2 * b2_pi + b2_epsilon)
			angle -= 2 * b2_pi;
		while (side_angle > 2 * b2_pi + b2_epsilon)
			angle -= 2 * b2_pi;
		auto side_func_params = calculate_linear_function_params(pointA, pointB);
		if (side_angle == b2_pi)
			if (fabs(pointA.x - contact_point.x ) > b2_epsilon)
				continue;
		else if (fabs(contact_point.y - contact_point.x * std::get<0>(side_func_params) - std::get<1>(side_func_params)) < b2_epsilon)
			continue;
		if (fabs(angle - side_angle) < 10 * DEG_TO_RAD)
			return false;
	}
	return true;
}

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
		b2WorldManifold manifold;
		contact->GetWorldManifold(&manifold);
		msg.point = manifold.points[0];
		msg.normal = manifold.normal;
		contact_queue.push_back(msg);
	}
}

void destroys_upon_collision::send_message(abstract_entity* source) {
	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
	for (auto& msg : contact_queue) {
		if (msg.fixtureA->GetBody() == (b2Body*)0xFDFDFDFD || msg.fixtureB->GetBody() == (b2Body*)0xFDFDFDFD) {
			msg.delete_this_message = true;
			continue;
		}

		if (msg.contact_type == contact_type::AABB_CONTACT)
			continue;

		auto bodyA = (physical_entity*)msg.fixtureA->GetBody()->GetUserData();
		auto bodyB = (physical_entity*)msg.fixtureB->GetBody()->GetUserData();

		if (bodyA == source || bodyB == source) {
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

void explodes_upon_collision::send_message(abstract_entity* source) {
	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
	for (auto& msg : contact_queue) {
		if (msg.fixtureA->GetBody() == (b2Body*)0xFDFDFDFD || msg.fixtureB->GetBody() == (b2Body*)0xFDFDFDFD) {
			msg.delete_this_message = true;
			continue;
		}

		if (msg.contact_type == contact_type::AABB_CONTACT)
			continue;

		auto bodyA = (physical_entity*)msg.fixtureA->GetBody()->GetUserData();
		auto bodyB = (physical_entity*)msg.fixtureB->GetBody()->GetUserData();

		if (bodyA == source || bodyB == source) {
			for (float i = 0; i < num_rays; ++i) {
				float angle = (i / num_rays * 360 * DEG_TO_RAD);
				b2Vec2 ray_direction(sinf(angle), cosf(angle) );
				b2Vec2 ray_end = source->get_physical_body()->GetWorldCenter() + blast_radius * ray_direction;
				closest_ray_callback callback;
				cosmos.world.RayCast((b2RayCastCallback*)&callback, source->get_physical_body()->GetWorldCenter(), ray_end);
				if (callback.m_body) {
					apply_blast_impulse(callback.m_body, source->get_physical_body()->GetWorldCenter(), callback.contact_point, (blast_power / num_rays));
					//also produce "BANG!" image
					sf::RenderWindow useless_window;
					sf::Drawable* bang_sprite = new sf::Sprite(cosmos.resources.textures[5]);

					static_cast<sf::Sprite*>(bang_sprite)->setOrigin(sf::Vector2f(320.f, 220.f));
					static_cast<sf::Sprite*>(bang_sprite)->setScale(sf::Vector2f(0.2f, 0.2f));
					static_cast<sf::Sprite*>(bang_sprite)->setPosition(sf::Vector2f(source->get_physical_body()->GetPosition().x * SCALE, source->get_physical_body()->GetPosition().y * SCALE) );

					image_entity* bang = new image_entity(bang_sprite, "BANG!", image_entity::ILLUSION, useless_window);
					auto& illu_queue = cosmos.message_queues.get_queue<show_illusion_message>();
					show_illusion_message msg;
					msg.illusion_entity = bang;
					msg.duration = 250;
					illu_queue.push_back(msg);
				}
			}
		}
	}
}


//void desctructable_upon_collison::send_message(abstract_entity* source) {
//	auto& contact_queue = cosmos.message_queues.get_queue<contact_message>();
//	for (auto& msg : contact_queue) {
//		if (msg.fixtureA->GetBody() == (b2Body*)0xFDFDFDFD || msg.fixtureB->GetBody() == (b2Body*)0xFDFDFDFD) {
//			msg.delete_this_message = true;
//			continue;
//		}
//
//		if (msg.contact_type == contact_type::AABB_CONTACT)
//			continue;
//
//		auto bodyA = (physical_entity*)msg.fixtureA->GetBody()->GetUserData();
//		auto bodyB = (physical_entity*)msg.fixtureB->GetBody()->GetUserData();
//
//		if (bodyA == source || bodyB == source) {
//			msg.delete_this_message = true;
//			physical_entity* bullet;
//			b2Fixture* destructable_fixture;
//			if (bodyA == source) {
//				bullet = bodyB;
//				destructable_fixture = msg.fixtureA;
//			}
//			else {
//				bullet = bodyA;
//				destructable_fixture = msg.fixtureB;
//			}
//			if (destructable_fixture->GetShape()->GetType() != b2Shape::Type::e_polygon)
//				continue;
//			const auto& closest_scar = std::min_element(scars.begin(), scars.end(), [&msg](const auto& a, const auto& b) {
//				return b2Vec2(utillity::vec_to_b2vec(std::dynamic_pointer_cast<sf::RectangleShape>(a.visual_object)->getPosition()) - msg.point).Length() < b2Vec2(utillity::vec_to_b2vec(std::dynamic_pointer_cast<sf::RectangleShape>(b.visual_object)->getPosition()) - msg.point).Length();
//			});
//			if (scars.size() < max_scars_count && (closest_scar == scars.end() || 
//				b2Vec2(utillity::vec_to_b2vec(std::dynamic_pointer_cast<sf::RectangleShape>(closest_scar->visual_object)->getPosition()) - msg.point).Length() > scar_pos_epsilon )) 
//			{
//				float line_len = bullet->get_physical_body()->GetLinearVelocity().Length() * SCALE * TEST_SCALE_DOWN;
//				float angle = acos(bullet->get_physical_body()->GetLinearVelocity().x / bullet->get_physical_body()->GetLinearVelocity().Length()) + b2_pi;
//				if (!minimum_angle_test(destructable_fixture, msg.point, angle))
//					return;
//				std::shared_ptr<sf::RectangleShape> shape(new sf::RectangleShape);
//				shape->setSize(sf::Vector2f(line_len, 5));
//				shape->setPosition(sf::Vector2f(msg.point.x * SCALE, msg.point.y * SCALE ));
//				shape->setFillColor(sf::Color::Cyan);
//				shape->setRotation(angle * RAD_TO_DEG);
//				visual_effect scar{ visual_effect_type::LINE, shape, angle, bullet->get_physical_body()->GetLinearVelocity().Length() * TEST_SCALE_DOWN, msg.point };
//				static_cast<physical_entity*>(source)->addVisualEffect(scar);
//				scars.push_back(scar);
//			}
//			else {
//				//we have to expand the closest scar 
//				auto line = std::dynamic_pointer_cast<sf::RectangleShape>(closest_scar->visual_object);
//				line->setSize(sf::Vector2f(line->getSize().x + bullet->get_physical_body()->GetLinearVelocity().Length() * SCALE * TEST_SCALE_DOWN, line->getSize().y));
//				closest_scar->property2 += bullet->get_physical_body()->GetLinearVelocity().Length() * TEST_SCALE_DOWN;
//				b2PolygonShape local_shape = *static_cast<b2PolygonShape*>(destructable_fixture->GetShape());
//				b2RayCastOutput raycast_output;
//				b2RayCastInput raycast_input;
//				raycast_input.p2 = closest_scar->vector_property;
//				raycast_input.p1 = utillity::rotate(b2Vec2(closest_scar->vector_property.x + closest_scar->property2, closest_scar->vector_property.y), closest_scar->vector_property, closest_scar->property);
//				raycast_input.p2 -= source->get_physical_body()->GetPosition();
//				raycast_input.p1 -= source->get_physical_body()->GetPosition();
//				raycast_input.maxFraction = 1.0f;
//				b2Transform dude_weed_lmao;
//				dude_weed_lmao.SetIdentity();
//				if (local_shape.RayCast(&raycast_output, raycast_input, dude_weed_lmao, 0)) {
//					printf("RAYCAST SUCCEED\n");
//					//split();
//					//static_cast<physical_entity*>(source)->removeVisualEffect(closest_scar._Ptr);
//					//scars.erase(closest_scar);
//				}
//	 		}
//		}
//	}
//}