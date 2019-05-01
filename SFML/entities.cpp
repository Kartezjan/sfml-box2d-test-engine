#include "entities.h"
#include "window.h"
#include "animation.h"

image_entity::image_entity(sf::Drawable* object, const sf::Vector2f pos,  const std::string& n_name, const content_type n_type, sf::RenderWindow& win_ref) : window(win_ref) {
	type = entity_type::IMAGE;
	visual_object = object;
	name = n_name;
	image_type = n_type;
	position_ = pos;
}

void image_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	if (!hidden_)
	{
		states.transform *= getTransform();
		target.draw(*visual_object, states);
	}
}

void image_entity::update() {
	auto transformable = dynamic_cast<sf::Transformable*>(visual_object);
	if(sticky_)
	{
		map_to_window(*transformable, position_, window);
	}
	else
	{
		transformable->setPosition(position_);
	}
	animation_resource* anim = nullptr;
	switch(image_type)
	{
	case content_type::ANIMATION:
		anim = dynamic_cast<animation_resource*>(visual_object);
		assert(anim);
		anim->update();
		break;
	default:
		break;
	}
}

primitive_entity::primitive_entity(const body_properties& body_properties, std::string n_name, const sf::Texture& box_texture) : texture(box_texture) {
	type = entity_type::PHYSICAL;
	name = n_name;
	physical_body = create_physical_body(body_properties, dynamic_cast<physical_entity*>(this));
	shapes = utillity::convert_shape_to_sf(body_properties, box_texture);
}

primitive_entity::~primitive_entity() {
	for (auto& obj : shapes)
		delete obj.visual_object;
	for (auto& obj : visual_effects)
		delete obj.visual_object;
}

void primitive_entity::expand_texture_rect()
{
	for(auto& shape : shapes)
	{
		auto obj = dynamic_cast<sf::ConvexShape*>(shape.visual_object);
		if(obj)
		{
			const auto bounds = obj->getLocalBounds();
			obj->setTextureRect(sf::IntRect{bounds});
		}
	}
}

void primitive_entity::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	states.transform *= getTransform();
	states.texture = &texture;
	for (auto& shape : shapes)
		target.draw(*shape.visual_object, states);
	for (auto& vis_eff : visual_effects)
		target.draw(*vis_eff.visual_object);
}

void primitive_entity::update() {
	size_t shape_count = shapes.size() - 1;;

	for (auto fixture = physical_body->GetFixtureList(); fixture; fixture = fixture->GetNext()) {
		auto pos = get_shape_position(fixture->GetShape());
		auto body_pos = physical_body->GetPosition();
		pos += body_pos;
		utillity::rotate(pos, b2Vec2(body_pos.x, body_pos.y), physical_body->GetAngle());
		if (fixture->GetShape()->m_type == fixture->GetShape()->e_polygon) {
			auto current_shape = dynamic_cast<sf::ConvexShape*>(shapes[shape_count].visual_object);
			current_shape->setPosition(sf::Vector2f(pos.x * SCALE, pos.y * SCALE));
			current_shape->setRotation(physical_body->GetAngle() * 180 / b2_pi);
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