#pragma once

#include <optional>

#include "observer_ptr.h"
#include "ptr_vec.h"
#include "virtue.h"
#include "virtue_management.h"

using entity_id = std::size_t;

class _entity
{
public:
	_entity(const entity_id id) : id_(id) {}
	_entity(const _entity& other, const entity_id new_id) : id_(new_id)
	{
		virtues_ = deep_copy_vec_ptr(other.virtues_);
	}
	template <typename... CArgs>
	auto add_virtue(const virtue_type virtue_to_add, CArgs&&... args) -> observer_ptr<virtue*>
	{
		const auto& res = virtues_.try_emplace(virtue_to_add, std::move(make_virtue(virtue_to_add, *this, std::forward(args)...)));
		return res.first->second.get();
	}
	bool remove_virtue(const virtue_type virtue_to_remove) {  return virtues_.erase(virtue_to_remove) > 0; }
	bool has_virtue(const virtue_type type)
	{
		const auto& res = virtues_.find(type);
		return res != virtues_.end();
	}
	virtue* get_virtue(const virtue_type type)
	{
		const auto& res = virtues_.find(type);
		return (res != virtues_.end()) ? res->second.get() : nullptr;
	}
	entity_id id() const { return id_; }
private:
	virtues_map virtues_;
	const entity_id id_;
};