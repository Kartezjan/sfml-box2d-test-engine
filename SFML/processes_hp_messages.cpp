#include "processes_hp_messages.h"

#include "sprite_entity.h"

void processes_hp_messages::send_message(abstract_entity* source)
{
	auto& hp_queries = cosmos.message_queues.get_queue<hit_points_modifier_message>();
	for (auto& msg : hp_queries)
	{
		msg.delete_this_message = true;
		auto target = dynamic_cast<sprite_entity*>(cosmos.all_entities[msg.target].get());
		if(!target)
		{
			return;
		}
		target->modify_hp(msg.delta);
	}
}
