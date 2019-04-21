#include "trebuchet.h"
#include "car.h"
#include "camera.h"

void setup_trebuchet(primitive_entity& main_body, primitive_entity& beam, /*primitive_entity& projectile,*/ primitive_entity& counter_weight, primitive_entity& grand_wheel, b2Vec2 beam_pos, b2Vec2 counter_weight_pos, universe& cosmos) {
	grand_wheel.get_physical_body()->SetFixedRotation(true);
	grand_wheel.virtues.push_back(std::make_unique<gear_controll>(cosmos));

	b2RevoluteJointDef revoluteJointDef_beam;
	revoluteJointDef_beam.bodyA = main_body.get_physical_body();
	revoluteJointDef_beam.bodyB = beam.get_physical_body();
	revoluteJointDef_beam.localAnchorA.Set(beam_pos.x / SCALE, beam_pos.y / SCALE);
	revoluteJointDef_beam.localAnchorB.Set(0, 0);

	b2RevoluteJointDef revoluteJointDef_wheel;
	revoluteJointDef_wheel.bodyA = main_body.get_physical_body();
	revoluteJointDef_wheel.bodyB = grand_wheel.get_physical_body();
	revoluteJointDef_wheel.localAnchorA.Set(beam_pos.x / SCALE, (beam_pos.y + 200) / SCALE);
	revoluteJointDef_beam.localAnchorB.Set(0, 0);

	b2GearJointDef gear;
	gear.bodyA = beam.get_physical_body();
	gear.bodyB = grand_wheel.get_physical_body();
	gear.ratio = 0.1f;


	b2RopeJointDef ropeJointDef_counter_weight;
	ropeJointDef_counter_weight.bodyA = beam.get_physical_body();
	ropeJointDef_counter_weight.bodyB = counter_weight.get_physical_body();
	ropeJointDef_counter_weight.localAnchorA.Set(counter_weight_pos.x / SCALE, counter_weight_pos.y / SCALE);
	ropeJointDef_counter_weight.localAnchorB.Set(0, 0);
	ropeJointDef_counter_weight.maxLength = 100 / SCALE;


	auto beam_axis = cosmos.world.CreateJoint(&revoluteJointDef_beam);
	auto wheel_axis = cosmos.world.CreateJoint(&revoluteJointDef_wheel);

	gear.joint1 = beam_axis;
	gear.joint2 = wheel_axis;
	
	cosmos.world.CreateJoint(&gear);
	cosmos.world.CreateJoint(&ropeJointDef_counter_weight);

	//add rope visuals
	sf::ConvexShape* rope = new sf::ConvexShape;
	float thickness = 10.0f / 2;
	rope->setPointCount(4);
	rope->setFillColor(sf::Color::Red);

	visual_effect effect;
	effect.type = visual_effect_type::ROPE_JOINT;
	effect.visual_object = rope;
	effect.property = thickness;
	
	beam.addVisualEffect(effect);
}

void trebuchet_spawn(universe& universe)
{
	const auto player_handle = universe.all_entities += new primitive_entity(create_player(universe.world, 370, 350), "player", universe.resources.textures_["box"]);
	const auto front_wheel_handle = universe.all_entities += new primitive_entity(create_circle(universe.world, 370, 350, 20.f, 500.f, 0.7f), "front_wheel", universe.resources.textures_["wheel"]);
	const auto back_wheel_handle = universe.all_entities += new primitive_entity(create_circle(universe.world, 370, 350, 20.f, 500.f, 0.7f), "back_wheel", universe.resources.textures_["wheel"]);
	auto player = dynamic_cast<physical_entity*>(universe.all_entities.access(player_handle).get());
	auto front_wheel = dynamic_cast<physical_entity*>(universe.all_entities.access(front_wheel_handle).get());
	auto back_wheel = dynamic_cast<physical_entity*>(universe.all_entities.access(back_wheel_handle).get());
	setup_car(*player, *front_wheel, *back_wheel, b2Vec2(-280.f,-10.f), b2Vec2(280.f,-10.f), 50000, universe);
	player->virtues.push_back(std::make_unique<center_of_attention>(universe));

	auto beam_handle = universe.all_entities += new primitive_entity (create_box(universe.world, 370, -200, 1000, 20, b2Vec2(-100,0), 0, 30.f, 0.7f), "trebuchet_beam", universe.resources.textures_["box"]);
	auto cw_handle = universe.all_entities += new primitive_entity (create_box(universe.world, 668, -150, 70, 70, 500, 0.7f), "TREB_CW", universe.resources.textures_["box"]);
	auto gw_handle = universe.all_entities += new primitive_entity (create_circle(universe.world, 370, 90, 84, 1.0f, 0.1f), "TREB_WHEEL", universe.resources.textures_["box"]);
	auto beam = dynamic_cast<primitive_entity*>(universe.all_entities[beam_handle].get());
	auto cw = dynamic_cast<primitive_entity*>(universe.all_entities[cw_handle].get());
	auto gw = dynamic_cast<primitive_entity*>(universe.all_entities[gw_handle].get());
	setup_trebuchet(*dynamic_cast<primitive_entity*>(player), *beam, *cw, *gw, b2Vec2(0, -500), b2Vec2(400,0), universe);
}

void gear_controll::send_message(abstract_entity* source) {
	auto entity = dynamic_cast<physical_entity*>(source);
	assert(entity);
	auto& input_messages = cosmos.message_queues.get_queue<input_message>();
	for (auto& msg : input_messages) {
		if (msg.key == input_key::E) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp > 300) {
				previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				if (entity->get_physical_body()) {
					entity->get_physical_body()->SetFixedRotation(status);
					status = !status;
				}
			}
		}
		else if (msg.key == input_key::Q) {
			msg.delete_this_message = true;
			if (entity->get_physical_body()) {
				for (auto joint_edge = entity->get_physical_body()->GetJointList(); joint_edge; joint_edge = joint_edge->next ) {
					if (joint_edge->joint->GetType() != b2JointType::e_gearJoint)
						continue;
					if(joint_edge->joint->GetBodyA()->GetAngle() > -0.55f)
						entity->get_physical_body()->SetTransform(entity->get_physical_body()->GetPosition(), entity->get_physical_body()->GetAngle() + 15 * DEG_TO_RADf);
				}
				}
			}
	}
}
