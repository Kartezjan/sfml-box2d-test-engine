#include "sprite_entity.h"

void sprite_entity::update()
{
	b2Vec2 position_sum = {0,0};
	auto count = 0;
	b2Vec2 body_pos = {0,0};
	b2Vec2 centroid_sum = {0,0};
	for (auto fixture = physical_body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		auto pos = get_shape_position(fixture->GetShape());
		body_pos = physical_body->GetPosition();
		centroid_sum += pos;
		pos += body_pos;
		utillity::rotate(pos, body_pos, physical_body->GetAngle());
		position_sum += pos;
		++count;
	}
	assert(count);
	const auto mean_pos = to_sf_vec(position_sum) / static_cast<float>(count);
	const auto mean_centroid = to_sf_vec(centroid_sum) / static_cast<float>(count);
	animation_.update();
	shape_.setTexture(&animation_.get_current_frame());
	shape_.setPosition(to_sf_vec(physical_body->GetPosition()) * SCALE);
}

void sprite_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(shape_, states);
}
