#include "sprite_entity.h"

void sprite_entity::update()
{
	b2Vec2 position_sum = {0,0};
	auto count = 0;
	b2Vec2 body_pos = {0,0};
	b2Vec2 centroid_sum = {0,0};
	for (auto fixture = physical_body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		if (static_cast<fixture_data*>(fixture->GetUserData())->fixture_type != fixture_data::standard)
		{
			// Don't account auxiliary fixtures.
			continue;
		}
		auto pos = get_shape_position(fixture->GetShape());
		body_pos = physical_body->GetPosition();
		centroid_sum += pos;
		pos += body_pos;
		utillity::rotate(pos, body_pos, physical_body->GetAngle());
		position_sum += pos;
		++count;
	}
	if (count) {
		const auto mean_pos = to_sf_vec(position_sum) / static_cast<float>(count);
		const auto mean_centroid = to_sf_vec(centroid_sum) / static_cast<float>(count);
		const auto size = shape_.getTextureRect();
		if (flip_)
		{
			shape_.setScale(-1.f, 1.f);
		}
		else
		{
			shape_.setScale(1.f, 1.f);
		}
		shape_.setPosition(mean_pos * SCALE);
	}
	shape_.setTexture(&animation_.get_current_frame());
	animation_.update();
}

void sprite_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape_, states);
}
