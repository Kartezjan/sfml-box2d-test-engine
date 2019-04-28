#include <set>

#include "scanner.h"

auto scanner::scan(physical_entity* source,  const std::pair<float,float> angle_range, const b2Vec2 offset, const bool sight_mode) const -> std::set<entity_id>
{
	const auto body = source->get_physical_body();
	const auto body_pos = body->GetPosition();
	auto found_bodies = std::set<entity_id>{};
	auto current_angle = angle_range.first;
	for(auto i = 0; i < static_cast<int>(ceil(angle_range.second - angle_range.first) / delta_); ++i)
	{
		b2RayCastInput input;
		const auto end_point = body_pos + offset + range_ * b2Vec2{ cosf(current_angle), sinf(current_angle) };
		input.p1 = body_pos + offset;
		input.p2 = end_point;
		input.maxFraction = 1.f;
		auto closest_fraction = 1.f;
		auto closest_entity = entity_id{ 0 };
		for (auto b = cosmos_.world.GetBodyList(); b; b = b->GetNext())
		{
			for (auto f = b->GetFixtureList(); f; f = f->GetNext())
			{
				const auto id = static_cast<physical_entity*>(b->GetUserData())->id();
				auto output = b2RayCastOutput{};
				if (f->RayCast(&output, input, 0) && source->id() != id)
				{
					if(sight_mode)
					{
						if(output.fraction < closest_fraction)
						{
							closest_fraction = output.fraction;
							closest_entity = id;
						}
					}
					else
					{
						found_bodies.emplace(id);
					}
				}
			}
		}
		if(sight_mode && closest_entity)
		{
			found_bodies.emplace(closest_entity);
		}
		current_angle += delta_;
	}
	return found_bodies;
}