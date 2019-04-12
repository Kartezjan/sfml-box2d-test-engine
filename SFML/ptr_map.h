#pragma once

#include <unordered_map>
#include <memory>
#include <optional>

typedef size_t entity_id;

template <typename val>
struct ptr_map 
{
	void operator+=(val&& elem) { map_.emplace(map_.size(), std::make_shared<>(elem)); }
	auto try_access(entity_id idx) -> std::<std::shared_ptr<val>>
	{
	}
	auto access(entity_id idx) -> std::shared_ptr<T> { return map_[idx]; }
private:
	std::unordered_map<entity_id, std::shared_ptr<val>> map_;
};
