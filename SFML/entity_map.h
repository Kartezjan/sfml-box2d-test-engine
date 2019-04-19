#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

#include "entity.h"

typedef size_t entity_id;

struct entity_map
{
	entity_id operator+=(_entity elem) { return map_.emplace(map_.size(), std::move(elem)).first->first; }
	auto spawn(const _entity elem) { return operator+=(elem); }
	auto operator-=(entity_id idx) { map_.erase(idx); }
	auto try_access(entity_id idx)
	{
		auto found = map_.find(idx);
		return found != map_.end() ? std::optional(found->second) : std::nullopt;
	}
	auto access(entity_id idx) -> std::shared_ptr<_entity> { return map_[idx]; }
	auto exists(entity_id idx) { return map_.find(idx) != map_.end(); }
private:
	std::unordered_map<entity_id, std::shared_ptr<_entity>> map_;
};
