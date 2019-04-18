#pragma once

#include "config.h"
#include "virtue.h"
#include "world.h"
#include "entity.h"
#include "physics.h"

class has_physics : public virtue
{
public:
	has_physics(universe& universe_ref, _entity& entity_ref, const body_properties& definition) : virtue(universe_ref, entity_ref) { add_physical_body(definition); }
	~has_physics() { remove_body(); }
	observer_ptr<b2Body> get_physical_body_() const { return physical_body_.get(); };
	void remove_body() 
	{
		if (physical_body_ != nullptr)
			physical_body_->GetWorld()->DestroyBody(physical_body_.get());
		physical_body_ = nullptr;
	}
	void add_physical_body(body_properties definition)
	{
		const auto ptr = create_physical_body(definition, &info_);
		physical_body_ = std::unique_ptr<b2Body>(ptr);
	}
protected:
	std::unique_ptr<b2Body> physical_body_ = nullptr; 
	entity_info info_ = {owner_ref_.id()};
};
