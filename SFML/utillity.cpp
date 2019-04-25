#include "utillity.h"

std::vector<shape> utillity::convert_shape_to_sf(const body_properties& properties, const sf::Texture& texture)
{
	auto shapes = std::vector<shape>{};
	for (auto& fixture : properties.fixtures) {
		auto current = shape{};
		if (fixture.second.shape->m_type == fixture.second.shape->e_polygon) {
			current.type = shape_type::CONVEX;
			const b2PolygonShape shape = *(b2PolygonShape*)fixture.second.shape;
			sf::ConvexShape* convex = new sf::ConvexShape;
			convex->setPointCount(shape.m_count);
			for (auto i = 0; i < shape.m_count; ++i)
				convex->setPoint(i, sf::Vector2f(shape.m_vertices[i].x * SCALE, shape.m_vertices[i].y * SCALE));
			convex->setOrigin(sf::Vector2f(shape.m_centroid.x * SCALE, shape.m_centroid.y * SCALE));
			convex->setTexture(&texture);
			current.visual_object = convex;
			shapes.push_back(current);
		}
		else {
			current.type = shape_type::CIRCLE;
			sf::CircleShape* circle = new sf::CircleShape;
			const b2CircleShape shape = *(b2CircleShape*)fixture.second.shape;
			circle->setRadius(shape.m_radius * SCALE);
			circle->setOrigin(sf::Vector2f(shape.m_radius * SCALE, shape.m_radius * SCALE));
			circle->setTexture(&texture);
			current.visual_object = circle;
			shapes.push_back(current);
		}
	}
	return shapes;
}
