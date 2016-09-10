#include "car.h"

void setup_car(physical_entity& body_car, physical_entity& wheel_front, physical_entity& wheel_back, b2Vec2 front_wheel_pos, b2Vec2 back_wheel_pos, float maxTorgue ,universe& cosmos) {
	b2RevoluteJointDef revoluteJointDef_front_wheel;
	b2RevoluteJointDef revoluteJointDef_back_wheel;

	revoluteJointDef_front_wheel.bodyA = body_car.get_physical_body();
	revoluteJointDef_front_wheel.bodyB = wheel_front.get_physical_body();
	revoluteJointDef_front_wheel.localAnchorA.Set(front_wheel_pos.x / SCALE, front_wheel_pos.y / SCALE);
	revoluteJointDef_front_wheel.localAnchorB.Set(0, 0);
	revoluteJointDef_front_wheel.maxMotorTorque = maxTorgue;

	revoluteJointDef_back_wheel.bodyA = body_car.get_physical_body();
	revoluteJointDef_back_wheel.bodyB = wheel_back.get_physical_body();
	revoluteJointDef_back_wheel.localAnchorA.Set(back_wheel_pos.x / SCALE, back_wheel_pos.y / SCALE);
	revoluteJointDef_back_wheel.localAnchorB.Set(0, 0);
	revoluteJointDef_back_wheel.maxMotorTorque = maxTorgue;
	revoluteJointDef_back_wheel.motorSpeed = 0;


	cosmos.world.CreateJoint(&revoluteJointDef_front_wheel);
	cosmos.world.CreateJoint(&revoluteJointDef_back_wheel);

	body_car.relatives.push_back(&wheel_front);
	body_car.relatives.push_back(&wheel_back);

	body_car.virtues.push_back(new controllable_car(cosmos, 300 * DEG_TO_RAD));
}

void controllable_car::send_message(abstract_entity* source) {
	if (source->get_type() == entity_type::PHYSICAL) {
		auto input_messages = cosmos.message_queues.get_queue<input_message>();
		for (auto msg : input_messages) {
			if (msg.key == input_key::W || msg.key == input_key::S) {
					for (auto relative : source->relatives) {
						if (relative->name == "front_wheel" || relative->name == "back_wheel") {
							auto front_wheel = (physical_entity*)relative;
							b2RevoluteJoint* joint = (b2RevoluteJoint*)front_wheel->get_physical_body()->GetJointList()->joint;
							if (joint) {
								joint->EnableMotor(true);
								if (msg.key == input_key::W)
									joint->SetMotorSpeed(joint->GetMotorSpeed() + acceleration);
								else if (msg.key == input_key::S)
									joint->SetMotorSpeed(joint->GetMotorSpeed() - acceleration);
							}
						}
					}

			}
			if (msg.key == input_key::SPACE) {
				for (auto relative : source->relatives) {
					is_space_pressed = true;
					if (relative->name == "front_wheel" || relative->name == "back_wheel") {
						auto front_wheel = (physical_entity*)relative;
						b2RevoluteJoint* joint = (b2RevoluteJoint*)front_wheel->get_physical_body()->GetJointList()->joint;
						joint->EnableMotor(true);
						joint->SetMotorSpeed(0);
					}
				}
			}
			else if (is_space_pressed) {
				is_space_pressed = false;
				for (auto relative : source->relatives) {
					if (relative->name == "front_wheel" || relative->name == "back_wheel") {
						auto front_wheel = (physical_entity*)relative;
						b2RevoluteJoint* joint = (b2RevoluteJoint*)front_wheel->get_physical_body()->GetJointList()->joint;
						joint->EnableMotor(false);
					}
				}
			}
		}
	}
	else
		printf("Only a physical entity can be a car\n");
}