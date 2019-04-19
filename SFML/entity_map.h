#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

#include "entity.h"

typedef size_t entity_id;

struct entity_map
{
	auto spawn() {  return map_.emplace(map_.size(), _entity{map_.size()}).first->first; }
	auto operator()() { return spawn(); }
	auto operator-=(const entity_id idx) { map_.erase(idx); }
	auto try_access(const entity_id idx)
	{
		auto found = map_.find(idx);
		return found != map_.end() ? std::optional(found->second) : std::nullopt;
	}
	auto access(const entity_id idx) -> std::shared_ptr<_entity> { return map_[idx]; }
	auto operator[](const entity_id idx) { return access(idx); }
	auto exists(const entity_id idx) { return map_.find(idx) != map_.end(); }
private:
	std::unordered_map<entity_id, std::shared_ptr<_entity>> map_;
};
