#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

typedef size_t entity_id;

struct ptr_map 
{
	void operator+=(val&& elem) { map_.emplace(map_.size(), std::make_unique<>(elem)); }
	auto try_access(entity_id idx) -> std::optional<val*>
	{
		map_.find()
	}
	auto access(entity_id idx) -> T* { return map_[idx].get(); }
private:
	std::unordered_map<entity_id, std::unique_ptr<val>> map_;
}; 
