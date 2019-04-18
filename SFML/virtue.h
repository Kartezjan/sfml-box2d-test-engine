#pragma once

#include "event_management.h"

enum class virtue_type
{ 
	empty_virtue = 0,
	controllable, 
	controllable_car,
	produces_user_input, 
	applies_force, 
	spawns_objects, 
	changes_gui_text, 
	controlls_view, 
	tracks_object, 
	center_of_attention,
	has_physical_body,
	count
};

struct universe;
class _entity;

const auto discard_all_messages = [&](auto& source, auto& a)
{
	if(a.fixture_a->GetBody() == source || a.fixture_b->GetBody() == source)
		a.delete_this_message = true;
};

class virtue {
public:
	explicit virtue(universe& uni_ref, _entity& entity_ref) : cosmos(uni_ref), owner_ref_(entity_ref) {}
	virtual ~virtue() = default;
	virtual void send_message(abstract_entity* source) = 0;
	_entity& owner() const { return owner_ref_; }
	
protected:
	universe& cosmos;
	_entity& owner_ref_;
};

using virtues_map = std::unordered_map<virtue_type, std::unique_ptr<virtue>>;
using virtues_vec = std::vector<std::unique_ptr<virtue>>;
