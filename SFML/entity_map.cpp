#include "entity_map.h"
#include "entities.h"

entity_id entity_map::operator+=(abstract_entity* elem)
{
	const auto result = map_.emplace(last_id_++, std::unique_ptr<abstract_entity>(elem));
	if (result.second)
	{
		result.first->second.get()->change_id(result.first->first);
	}
	return result.first->first;
}
