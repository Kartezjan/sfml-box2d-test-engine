#include "behavior.h"

#include "scanner.h"
#include "sprite_entity.h"
#include "contacts.h"

constexpr auto retreat_range = 400.f / SCALE;
constexpr auto retreat_speed = 12.f;
constexpr auto shooting_range = 800.f / SCALE;

bool stands_on_ground(universe& cosmos, const entity_id source)
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

bool can_move_in_specified_direction(physical_entity& who, universe& cosmos, const bool direction, const float center, const float range, const float offset_x, const bool ignore_range)
{
	const auto dir = direction ? 1.f : -1.f;
	if(!ignore_range &&  dir * (who.get_physical_body()->GetPosition().x - center) > range )
	{
		return false;
	}
	auto& queue = cosmos.message_queues.get_queue<contact_message>();
	const auto offset = direction ? b2Vec2{ offset_x / SCALE, 0 } : b2Vec2{ -offset_x / SCALE, 0 };
	scanner scan(cosmos, 1.f * DEG_TO_RADf, 70.f / SCALE);
	auto entity_set = scan.scan(&who, { 90.f * DEG_TO_RADf, 90.1f * DEG_TO_RADf }, offset, false);
	for (auto& entity : entity_set)
	{
		//if (cosmos.all_entities[entity]->get_name() == "ground")
		{
			return true;
		}
	}
	return false;
}


std::vector<entity_id> enemy_on_sight(physical_entity& who, universe& cosmos, const bool direction, std::pair<float, float> cone, const float range)
{
	std::vector<entity_id> result;
	scanner scan(cosmos, 2.f * DEG_TO_RADf, range);
	const auto offset = direction ? b2Vec2{ 20.f / SCALE, 0 } : b2Vec2{ -20.f / SCALE, 0 };
	if (!direction)
	{
		cone = { cone.first + 180 * DEG_TO_RADf, cone.second + 180 * DEG_TO_RADf };
	}
	auto entity_set = scan.scan(&who, cone, offset, true);
	for (auto& entity_id : entity_set)
	{
		auto entity = dynamic_cast<sprite_entity*>(cosmos.all_entities[entity_id].get());
		if(entity && entity->get_category() == sprite_entity::category::enemy)
		{
			result.emplace_back(entity_id);
		}
	}
	return result;
}

void ranger_behavior::send_message(abstract_entity* source)
{
	auto entity = dynamic_cast<sprite_entity*>(source);
	auto body = entity->get_physical_body();
	const auto current_velocity = body->GetLinearVelocity();
	physical_entity* closest_target = nullptr;
	auto closest_target_distance = shooting_range + 1.f;
	if(current_action_ == action::patrol && current_velocity.x < 1.f && current_velocity.x > -1.0f)
	{
		entity->select_animation_set(0);
	}
	if (!decision_tick_.can_use())
	{
		return;
	}
	const auto dir = direction_ ? 1 : -1;
	const auto desired_velocity = 5.f * dir;
	const auto delta = desired_velocity - current_velocity.x;
	auto impulse = delta * body->GetMass();
	auto& force_queue = cosmos.message_queues.get_queue<force_message>();
	auto can_attack = false;
	switch(current_action_)
	{
	case action::idle:
		break;
	case action::patrol:
		obstacle_ = false;
		targets_ = enemy_on_sight(*entity, cosmos, direction_, { -10 * DEG_TO_RADf, 10 * DEG_TO_RADf }, shooting_range);
		can_attack = !targets_.empty();
		if(can_attack)
		{
			current_action_ = action::attack;
			break;
		}
		can_move_ = can_move_in_specified_direction(*entity, cosmos, direction_, center_, range_, 60.f);
		if(can_move_)
		{
			force_queue.push_back(force_message
				{
					{
						false, 2
					},
					force_type::APPLY_IMPULS_TO_CENTER, b2Vec2{impulse, 0}, entity->id()
			});
			current_action_ = action::moving;
			entity->select_animation_set(1);
			entity->flip(!direction_);
		}
		else if(stands_on_ground(cosmos, source->id()))
		{
			force_queue.push_back(force_message
			{
				{
					false, 2
				},
				force_type::APPLY_IMPULS_TO_CENTER, b2Vec2{ (0 - current_velocity.x) * body->GetMass(), 0 }, entity->id()
			});
			direction_ = !direction_;
		}
		break;
	case action::moving:
		if(current_velocity.x < 1.f && current_velocity.x > -1.0f)
		{
			if (!retreats_)
			{
				direction_ = !direction_;
			}
			else 
			{
				obstacle_ = true;
			}
		}
		if(retreats_)
		{
			current_action_ = action::attack;
			direction_ = !direction_;
		}
		else
		{
			current_action_ = action::patrol;
		}
		retreats_ = false;
		break;
	case action::attack:
		closest_target_distance = shooting_range;
		for (auto& target_id : targets_)
		{
			auto target = dynamic_cast<physical_entity*>(cosmos.all_entities[target_id].get());
			const auto distance = target->get_physical_body()->GetPosition().x - entity->get_physical_body()->GetPosition().x;
			if(abs(closest_target_distance) > abs(distance))
			{
				closest_target_distance = distance;
				closest_target = target;
			}
			if (abs(distance) < retreat_range)
			{
				direction_ = distance < 0;
				can_move_ = can_move_in_specified_direction(*entity, cosmos, direction_, center_, range_, 120.f, true);
				if (!can_move_ || obstacle_)
				{
					force_queue.push_back
					(
						force_message
						{
							{
								false, 2
							},
							force_type::APPLY_IMPULS_TO_CENTER, b2Vec2{(0 - current_velocity.x) * entity->get_physical_body()->GetMass(), 0}, entity->id()
						}
					);
					retreats_ = false;
					break;
				}
				retreats_ = true;
				close_target_ = target_id;
				entity->flip(!direction_);
				const auto dir = direction_ ? 1 : -1;
				force_queue.push_back
				(
					force_message
					{
						{
							false, 2
						},
						force_type::APPLY_IMPULS_TO_CENTER, b2Vec2{(retreat_speed * dir - current_velocity.x) * entity->get_physical_body()->GetMass(), 0}, entity->id()
					}
				);
				entity->select_animation_set(1);
				current_action_ = action::moving;
				return;
			}
		}
		direction_ = closest_target_distance > 0;
		entity->flip(!direction_);
		entity->select_animation_set(2);
		attack_duration_.can_use();
		current_action_ = action::attacking;
		break;
	case action::attacking:
		if (attack_duration_.can_use())
		{
			current_action_ = action::patrol;
			obstacle_ = false;
		}
		else
		{
			shoot(entity);
		}
		break;
	default:
		break;
	}
	can_move_ = can_move_in_specified_direction(*entity, cosmos, direction_, center_, range_, 60.f);
	if(!can_move_&& stands_on_ground(cosmos, source->id()))
	{
		force_queue.push_back
		(
			force_message
			{
				{
					false, 2
				},
				force_type::APPLY_IMPULS_TO_CENTER, b2Vec2{(0 - current_velocity.x) * entity->get_physical_body()->GetMass(), 0}, entity->id()
			}
		);
	}
}

void ranger_behavior::shoot(physical_entity* source)
{
	constexpr float speed = 60.f;
	auto& force_queue = cosmos.message_queues.get_queue<force_message>();
	const auto angle = bullet_pattern_(cosmos.rng);
	const auto body = source->get_physical_body();
	const auto offset = direction_ ? std::pair<float, float>{80.f, 0.f} : std::pair<float, float>{ -80.f, 0.f };
	auto handle = cosmos.all_entities += new primitive_entity
	(
		create_circle(cosmos.world, 
		body->GetPosition().x * SCALE + offset.first,
		body->GetPosition().y * SCALE + offset.second,
		6.f, 1.f, 0.7f),
		"bullet", cosmos.resources.textures_["cyan"]
	);
	cosmos.all_entities[handle]->virtues.push_back(std::make_unique<destroys_upon_collision>(cosmos));
	cosmos.all_entities[handle]->virtues.push_back(std::make_unique<hp_removal_upon_collision>(cosmos, damage_pattern_(cosmos.rng), 50));
	auto bullet_body = dynamic_cast<primitive_entity*>(cosmos.all_entities[handle].get())->get_physical_body();
	const auto dir = direction_ ? 1 : -1;
	const auto force = b2Vec2{ dir * cosf(angle) * speed * bullet_body->GetMass(), sinf(angle) * speed * bullet_body->GetMass() };
	force_queue.push_back
	(
		force_message
		{
			{
				false, 2
			},
			force_type::APPLY_IMPULS_TO_CENTER, force, handle
		}
	);
}
