#include "car.h"
#include "has_physics.h"

void setup_car(_entity& body_car_entity, _entity& wheel_front_entity, _entity& wheel_back_entity, b2Vec2 front_wheel_pos, b2Vec2 back_wheel_pos, float maxTorgue ,universe& cosmos) {
	const auto body_virtue = dynamic_cast<has_physics*>(body_car_entity.get_virtue(virtue_type::has_physics).get());
	const auto wheel_front_virtue = dynamic_cast<has_physics*>(wheel_front_entity.get_virtue(virtue_type::has_physics).get());
	const auto wheel_back_virtue = dynamic_cast<has_physics*>(wheel_back_entity.get_virtue(virtue_type::has_physics).get());
	b2RevoluteJointDef revoluteJointDef_front_wheel;
	b2RevoluteJointDef revoluteJointDef_back_wheel;
	if (body_virtue && wheel_front_virtue && wheel_back_virtue)
	{
		auto body_car = body_virtue->physical_body().get();
		auto wheel_front = wheel_front_virtue->physical_body().get();
		auto wheel_back = wheel_back_virtue->physical_body().get();

		revoluteJointDef_front_wheel.bodyA = body_car;
		revoluteJointDef_front_wheel.bodyB = wheel_front;
		revoluteJointDef_front_wheel.localAnchorA.Set(front_wheel_pos.x / SCALE, front_wheel_pos.y / SCALE);
		revoluteJointDef_front_wheel.localAnchorB.Set(0, 0);
		revoluteJointDef_front_wheel.maxMotorTorque = maxTorgue;

		revoluteJointDef_back_wheel.bodyA = body_car;
		revoluteJointDef_back_wheel.bodyB = wheel_back;
		revoluteJointDef_back_wheel.localAnchorA.Set(back_wheel_pos.x / SCALE, back_wheel_pos.y / SCALE);
		revoluteJointDef_back_wheel.localAnchorB.Set(0, 0);
		revoluteJointDef_back_wheel.maxMotorTorque = maxTorgue;

		cosmos.world.CreateJoint(&revoluteJointDef_front_wheel);
		cosmos.world.CreateJoint(&revoluteJointDef_back_wheel);
		body_car_entity.add_virtue(virtue_type::controllable_car, cosmos, body_car, 120 * DEG_TO_RAD);
	} else
	{
		assert(false, "Entity has no physical body.");
	}
}

void controllable_car::process() {
	const auto body_virtue = dynamic_cast<has_physics*>(owner_ref_.get_virtue(virtue_type::has_physics).get());
	if (body_virtue) {
		const auto body = body_virtue->physical_body();
		auto& input_messages = cosmos_.message_queues.get_queue<input_message>();
		for (auto& msg : input_messages) {
			if (msg.key == input_key::W || msg.key == input_key::S || msg.key == input_key::SPACE) {
				msg.delete_this_message = true;
				for (auto joint_edge = body->GetJointList(); joint_edge; joint_edge = joint_edge->next) {
					const auto wheel = static_cast<physical_entity*>(joint_edge->joint->GetBodyB()->GetUserData());
					if (wheel->name == "front_wheel" || wheel->name == "back_wheel") {
						auto joint = dynamic_cast<b2RevoluteJoint*>(joint_edge->joint);
						joint->EnableMotor(true);
						if(msg.key == input_key::W)
							joint->SetMotorSpeed(joint->GetMotorSpeed() + acceleration_);
						else if(msg.key == input_key::S)
							joint->SetMotorSpeed(joint->GetMotorSpeed() - acceleration_);
						else if (msg.key == input_key::SPACE)
							joint->SetMotorSpeed(0);
					}
				}
			}
		}
	}
	else
		printf("Only a physical entity can be a car\n");
		
}