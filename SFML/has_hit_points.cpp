#include "has_hit_points.h"

void has_hit_points::send_message(abstract_entity* source)
{
	auto& hp_queue = cosmos.message_queues.get_queue<hit_points_modifier_message>();
	for (auto& msg : hp_queue)
	{
		if(msg.target == source->id())
		{
			hp_ += msg.delta;
			msg.delete_this_message = true;
		}
	}
}