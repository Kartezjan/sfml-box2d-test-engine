#include "entities.h"

abstract_entity::~abstract_entity() {
	if (physical_body)
		physical_body->GetWorld()->DestroyBody(physical_body);
}

image_entity::~image_entity() {
	delete visual_object;
}

image_entity::image_entity(sf::Drawable* object, std::string n_name, content_type n_type, sf::RenderWindow& win_ref) : window(win_ref) {
	type = entity_type::IMAGE;
	visual_object = object;
	name = n_name;
	image_type = n_type;
}

void image_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = 0;
	target.draw(*visual_object, states);
}

void image_entity::update() {
	if (image_type == content_type::TEXT) {
		sf::Text* text_object = (sf::Text*)visual_object;
		text_object->setPosition(window.mapPixelToCoords(sf::Vector2i(0, 0)));
		float scale = window.getView().getSize().x / window.getSize().x;
		text_object->setScale(sf::Vector2f(scale, scale));
	}
}

physical_entity::physical_entity(body_properties& body_properties, std::string n_name, sf::Texture& box_texture) {
	type = entity_type::PHYSICAL;
	texture = box_texture;
	name = n_name;
	physical_body = create_physical_body(body_properties, this);
	for (auto fixture : body_properties.fixtures) {
		shape current;
		if (fixture.shape->m_type == fixture.shape->e_polygon) {
			current.type = shape_type::CONVEX;
			const b2PolygonShape shape = *(b2PolygonShape*)fixture.shape;
			sf::ConvexShape* convex = new sf::ConvexShape;
			convex->setPointCount(shape.m_count);
			for (size_t i = 0; i < shape.m_count; ++i)
				convex->setPoint(i, sf::Vector2f(shape.m_vertices[i].x * SCALE, shape.m_vertices[i].y * SCALE));
			convex->setOrigin(sf::Vector2f(shape.m_centroid.x * SCALE, shape.m_centroid.y * SCALE));
			convex->setTexture(&texture);
			current.visual_object = convex;
			shapes.push_back(current);
		}
		else {
			current.type = shape_type::CIRCLE;
			sf::CircleShape* circle = new sf::CircleShape;
			const b2CircleShape shape = *(b2CircleShape*)fixture.shape;
			circle->setRadius(shape.m_radius * SCALE);
			circle->setOrigin(sf::Vector2f(shape.m_radius * SCALE, shape.m_radius * SCALE));
			circle->setTexture(&texture);
			current.visual_object = circle;
			shapes.push_back(current);
		}
	}
}

physical_entity::~physical_entity() {
	for (auto& obj : shapes)
		delete obj.visual_object;
	for (auto& obj : visual_effects)
		delete obj.visual_object;
}

void physical_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &texture;
	for (auto& shape : shapes)
		target.draw(*shape.visual_object, states);
	for (auto& vis_eff : visual_effects)
		target.draw(*vis_eff.visual_object);
}

void physical_entity::update() {
	size_t shape_count = shapes.size() - 1;;

	for (auto fixture = physical_body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		auto pos = get_shape_position(fixture->GetShape());
		auto body_pos = physical_body->GetPosition();
		pos += body_pos;
		utillity::rotate(pos, b2Vec2(body_pos.x, body_pos.y), physical_body->GetAngle());
		if (fixture->GetShape()->m_type == fixture->GetShape()->e_polygon) {
			static_cast<sf::ConvexShape*>(shapes[shape_count].visual_object)->setPosition(sf::Vector2f(pos.x * SCALE, pos.y * SCALE));
			static_cast<sf::ConvexShape*>(shapes[shape_count].visual_object)->setRotation(physical_body->GetAngle() * 180 / b2_pi);
		}
		else {
			static_cast<sf::CircleShape*>(shapes[shape_count].visual_object)->setPosition(sf::Vector2f(pos.x * SCALE, pos.y * SCALE));
			static_cast<sf::CircleShape*>(shapes[shape_count].visual_object)->setRotation(physical_body->GetAngle() * 180 / b2_pi);
		}
		--shape_count;
	}
	for (auto& vis_obj : visual_effects) {
		if (vis_obj.type == visual_effect_type::ROPE_JOINT) {
			sf::ConvexShape* shape = (sf::ConvexShape*)vis_obj.visual_object;
			for (auto joint_edge = physical_body->GetJointList(); joint_edge; joint_edge = joint_edge->next) {
				if (joint_edge->joint->GetType() != b2JointType::e_ropeJoint)
					continue;
				sf::Vector2f points[4];
				points[0] = sf::Vector2f(joint_edge->joint->GetAnchorA().x * SCALE - vis_obj.property, joint_edge->joint->GetAnchorA().y * SCALE - vis_obj.property);
				points[3] = sf::Vector2f(joint_edge->joint->GetAnchorA().x * SCALE + vis_obj.property, joint_edge->joint->GetAnchorA().y * SCALE + vis_obj.property);
				points[1] = sf::Vector2f(joint_edge->joint->GetAnchorB().x * SCALE - vis_obj.property, joint_edge->joint->GetAnchorB().y * SCALE - vis_obj.property);
				points[2] = sf::Vector2f(joint_edge->joint->GetAnchorB().x * SCALE + vis_obj.property, joint_edge->joint->GetAnchorB().y * SCALE + vis_obj.property);

				utillity::rotate(points[0], sf::Vector2f(joint_edge->joint->GetAnchorA().x * SCALE, joint_edge->joint->GetAnchorA().y * SCALE), 90);
				utillity::rotate(points[3], sf::Vector2f(joint_edge->joint->GetAnchorA().x * SCALE, joint_edge->joint->GetAnchorA().y * SCALE), 90);
				utillity::rotate(points[1], sf::Vector2f(joint_edge->joint->GetAnchorB().x * SCALE, joint_edge->joint->GetAnchorB().y * SCALE), 90);
				utillity::rotate(points[2], sf::Vector2f(joint_edge->joint->GetAnchorB().x * SCALE, joint_edge->joint->GetAnchorB().y * SCALE), 90);
			
				for (size_t i = 0; i < 4; ++i)
					shape->setPoint(i, points[i]);
			}
		}
	}
}