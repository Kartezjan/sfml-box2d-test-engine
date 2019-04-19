#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

typedef size_t entity_id;

template <typename val>
struct ptr_map
{
	entity_id operator+=(val& elem) { return map_.emplace(map_.size(), std::make_shared<val>(elem)).first->first; }
	auto spawn(const std::shared_ptr<val>& elem) { return operator+=(elem); }
	auto operator-=(entity_id idx) { map_.erase(idx); }
	auto try_access(entity_id idx) -> std::optional<std::shared_ptr<val>> 
	{
		auto found = map_.find(idx);
		return found.first ? std::optional(found.second) : std::nullopt;
	}
	auto access(entity_id idx) -> std::shared_ptr<val> { return map_[idx]; }
	auto exists(entity_id idx) { return map_.find(idx).first; }
private:
	std::unordered_map<entity_id, std::shared_ptr<val>> map_;
};
