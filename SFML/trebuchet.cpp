#include "trebuchet.h"

void setup_trebuchet(physical_entity& main_body, physical_entity& beam, /*physical_entity& projectile,*/ physical_entity& counter_weight, physical_entity& grand_wheel, b2Vec2 beam_pos, b2Vec2 counter_weight_pos, universe& cosmos) {
	grand_wheel.get_physical_body()->SetFixedRotation(true);
	grand_wheel.virtues.push_back(new gear_controll(cosmos));

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

void gear_controll::send_message(abstract_entity* source) {
	auto& input_messages = cosmos.message_queues.get_queue<input_message>();
	for (auto& msg : input_messages) {
		if (msg.key == input_key::E) {
			msg.delete_this_message = true;
			if (cosmos.universe_clock.getElapsedTime().asMilliseconds() - previous_timestamp > 300) {
				previous_timestamp = cosmos.universe_clock.getElapsedTime().asMilliseconds();
				if (source->get_physical_body()) {
					source->get_physical_body()->SetFixedRotation(status);
					status = !status;
				}
			}
		}
		else if (msg.key == input_key::Q) {
			msg.delete_this_message = true;
			if (source->get_physical_body()) {
				for (auto joint_edge = source->get_physical_body()->GetJointList(); joint_edge; joint_edge = joint_edge->next ) {
					if (joint_edge->joint->GetType() != b2JointType::e_gearJoint)
						continue;
					if(joint_edge->joint->GetBodyA()->GetAngle() > -0.55f)
						source->get_physical_body()->SetTransform(source->get_physical_body()->GetPosition(), source->get_physical_body()->GetAngle() + 15 * DEG_TO_RAD);
				}
				}
			}
	}
}