#pragma once

#include "entities.h"

class sprite_entity : public physical_entity
{
public:
	enum class category { ally, enemy, neutral };
	sprite_entity(const body_properties& properties, const animation_resource& anim, const float scale) : animation_(anim)
	{
		physical_body = create_physical_body(properties, dynamic_cast<physical_entity*>(this));
		auto sample_frame = animation_.get_current_frame();
		const auto size = sf::Vector2f(sample_frame.getSize());
		shape_ = sf::ConvexShape{ 4 };
		shape_.setPoint(0, sf::Vector2f{ -size.x/2, size.y/2 } * scale);
		shape_.setPoint(1, sf::Vector2f{ size.x/2, size.y/2 } * scale);
		shape_.setPoint(2, sf::Vector2f{ size.x/2, -size.y/2 } * scale);
		shape_.setPoint(3, sf::Vector2f{ -size.x/2, -size.y/2} * scale);
	}
	~sprite_entity()
	{
		physical_body->GetWorld()->DestroyBody(physical_body);
	}
	void select_animation_set(std::size_t id) { animation_.select_animation(id); };
	animation_element& get_current_animation() { return animation_.get_current_animation(); }
	void flip(const bool opt) { flip_ = opt; }
	void update() override;
	category get_category() const { return cat_; }
	void set_category(const category cat) { cat_ = cat; }
private:
	animation_resource animation_;
	sf::ConvexShape shape_;
	void draw(sf::RenderTarget&, sf::RenderStates states) const override;
	bool flip_ = false;
	bool flipped_ = false;
	category cat_ = category::neutral;
};
