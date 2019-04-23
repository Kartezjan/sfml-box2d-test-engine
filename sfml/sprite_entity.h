#pragma once

#include "entities.h"

class sprite_entity : public physical_entity
{
public:
	sprite_entity(const body_properties& properties, const animation_resource& anim) : animation_(anim)
	{
		physical_body = create_physical_body(properties, dynamic_cast<physical_entity*>(this));
		shape_ = sf::ConvexShape{ 4 };
		shape_.setPoint(0, { -18.5*3, 25*3 });
		shape_.setPoint(1, { 18.5*3, 25*3 });
		shape_.setPoint(2, { 18.5*3, -25*3 });
		shape_.setPoint(3, { -18.5*3, -25*3 });

	}
	~sprite_entity()
	{
		physical_body->GetWorld()->DestroyBody(physical_body);
	}
	void select_animation_set(std::size_t id) { animation_.select_animation(id); };
	void update() override;
private:
	animation_resource animation_;
	sf::ConvexShape shape_;
	void draw(sf::RenderTarget&, sf::RenderStates states) const override;
};
