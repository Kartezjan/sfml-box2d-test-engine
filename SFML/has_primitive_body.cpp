#include "has_primitive_body.h"

#include "entity.h"

void has_primitive_body::init(body_properties& props)
{
	const auto result = owner_ref_.add_virtue(virtue_type::has_physics, props);
	assert(result.second, "An entity already has physical body.");
	for (const auto& fixture : props.fixtures) {
		auto current = shape{};
		if (fixture.shape->m_type == fixture.shape->e_polygon) {
			current.type = shape_type::CONVEX;
			const auto shape = *dynamic_cast<const b2PolygonShape*>(fixture.shape);
			auto convex = new sf::ConvexShape;
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
			auto circle = new sf::CircleShape;
			const auto shape = *dynamic_cast<const b2CircleShape*>(fixture.shape);
			circle->setRadius(shape.m_radius * SCALE);
			circle->setOrigin(sf::Vector2f(shape.m_radius * SCALE, shape.m_radius * SCALE));
			circle->setTexture(&texture);
			current.visual_object = circle;
			shapes.push_back(current);
		}
	}
}
