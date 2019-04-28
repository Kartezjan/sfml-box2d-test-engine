#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

#include "observer_ptr.h"

class abstract_entity;

typedef size_t entity_id;

struct entity_map 
{
	auto operator-=(entity_id idx) { map_.erase(idx); }
	entity_id operator+=(abstract_entity* elem);
	auto try_access(entity_id idx) -> std::optional<observer_ptr<abstract_entity>>
	{
		const auto result = map_.find(idx);
		return result != map_.end() ? std::make_optional(observer_ptr<abstract_entity>(result->second.get())) : std::nullopt;
	}
	auto access(entity_id idx) -> observer_ptr<abstract_entity> { return map_[idx].get(); }
	auto operator[](entity_id idx) { return access(idx); }
	bool exists(entity_id id) { return map_.find(id) != map_.end(); }
	auto begin() { return map_.begin(); }
	auto end() { return map_.end(); }
private:
	std::unordered_map<entity_id, std::unique_ptr<abstract_entity>> map_;
	entity_id last_id_ = 1;
};
