#include "is_playable.h"
#include "sprite_entity.h"
#include "animation.h"
#include "scanner.h"

void is_playable::send_message(abstract_entity* source)
{
	const auto entity = dynamic_cast<sprite_entity*>(source);
	assert(entity);
	const auto body = entity->get_physical_body();
	assert(body);
	auto& input_queue = cosmos.message_queues.get_queue<input_message>();
	auto& force_queue = cosmos.message_queues.get_queue<force_message>();
	auto desired_velocity = 0.f;
	const auto current_velocity = body->GetLinearVelocity();
	auto apply = false;
	auto jump = false;
	bool cast_magick = false;
	auto stands = stands_on_ground(source->id());
	for(auto& msg : input_queue)
	{
		switch(msg.key)
		{
		case input_key::D:
			desired_velocity = 5;
			entity->flip(false);
			apply = true;
			inv_direction_ = false;
			msg.delete_this_message = true;
			break;
		case input_key::A:
			desired_velocity = -5;
			entity->flip(true);
			apply = true;
			inv_direction_ = true;
			msg.delete_this_message = true;
			break;
		case input_key::S:
			desired_velocity = 0;
			apply = true;
			msg.delete_this_message = true;
			break;
		case input_key::W:
			jump = jump_cooldown_.can_use() && stands;
			msg.delete_this_message = true;
			break;
		case input_key::SPACE:
			cast_magick = magick_cooldown_.can_use();
			msg.delete_this_message = true;
		default:
			break;
		}
		if(cast_magick)
		{
			// start casting
			entity->select_animation_set(5);
			casts_magick_ = true;
			magick_cast_duration_.can_use();
		}
		else if(apply && !casts_magick_)
		{
			const auto delta = desired_velocity - current_velocity.x;
			const auto impulse = body->GetMass() * delta;
			auto force_msg = force_message{};
			force_msg.target = source->id();
			force_msg.type = force_type::APPLY_IMPULS_TO_CENTER;
			force_msg.force = { impulse, 0. };
			force_queue.emplace_back(force_msg);
			msg.delete_this_message = true;
			if (stands)
			{
				entity->select_animation_set(1);
			}
		}
		else if(jump && !casts_magick_)
		{
			const auto impulse = body->GetMass() * -15.f;
			auto force_msg = force_message{};
			force_msg.target = source->id();
			force_msg.type = force_type::APPLY_IMPULS_TO_CENTER;
			force_msg.force = { 0., impulse };
			force_queue.emplace_back(force_msg);
		}
	}
	if(casts_magick_)
	{
		if(magick_cast_duration_.can_use())
		{
			casts_magick_ = false;
			entity->select_animation_set(0);
			// do magick n stuff
			const auto offset = inv_direction_ ? b2Vec2{-100 / SCALE, 0} : b2Vec2{ 100 / SCALE, 0 };
			const auto direction = inv_direction_ ? 180 : 0;
			const auto cone = std::pair<float,float>{ (direction - 45.f)*DEG_TO_RADf, (direction + 45.f) * DEG_TO_RADf };
			const auto power = -1.0e2f;
			auto bodies_in_range = scanner_.scan(entity, cone, offset);
			for (auto& current_entity : bodies_in_range)
			{
				const auto pos_diff = dynamic_cast<physical_entity*>(cosmos.all_entities[current_entity].get())->get_physical_body()->GetPosition() - body->GetPosition();
				const auto normal = b2Vec2{ cosf(pos_diff.x), sinf(pos_diff.y) };
				auto msg = force_message{ false, 2, force_type::APPLY_IMPULS_TO_CENTER, {normal.x * power, normal.y * power}, current_entity };
				force_queue.push_back(msg);
			}
		}
		// we are still casting
	}
	else if (stands)
	{
		if (current_velocity.x < 2.f && current_velocity.x > -2.f && !casts_magick_)
		{
			entity->select_animation_set(0);
		}
	}
	else if (current_velocity.y < 0.005f && current_velocity.y > -0.005f)
	{
		entity->select_animation_set(3);
		casts_magick_ = false;
	}
	else if (current_velocity.y < -1.0f)
	{
		entity->select_animation_set(4);
		casts_magick_ = false;
	}
	else
	{
		entity->select_animation_set(2);
		casts_magick_ = false;
	}
}

bool is_playable::stands_on_ground(const entity_id source) const
{
	auto& queue = cosmos.message_queues.get_queue<contact_message>();
	for(auto& msg : queue)
	{
		if (msg.contact_type == contact_type::AABB_CONTACT)
			continue;
		b2Fixture* target_fixture = nullptr;
		if(msg.owner_a == source)
		{
			target_fixture = msg.fixture_a;
		}
		else if (msg.owner_b == source)
		{
			target_fixture = msg.fixture_b;
		}
		else
		{
			continue;
		}
		if(const auto data = static_cast<fixture_data*>(target_fixture->GetUserData()))
		{
			if(data->fixture_type == fixture_data::foot)
			{
				// found contact with foot
				msg.delete_this_message = true;
				return true;
			}
		}
	}
	return false;
}
