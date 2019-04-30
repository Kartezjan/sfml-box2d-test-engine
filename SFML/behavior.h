#pragma once

#include <random>

#include "virtue_management.h"

bool can_move_in_specified_direction(physical_entity& who, universe& cosmos, const bool direction, const float center, const float range, const float offset_x, const bool ignore_range = false); // check if can move
enum class action{idle, patrol, attack, moving, attacking, retreat};

class ranger_behavior final : public virtue
{
public:
	ranger_behavior(universe& uni_ref, const float center, const float range, const std::pair<float,float> angle_range, const bool direction)
		: virtue(uni_ref), center_(center / SCALE), range_(range / SCALE), direction_(direction), angle_range_(angle_range), bullet_pattern_(angle_range_.first, angle_range_.second), damage_pattern_(-12, -4) { }
	void send_message(abstract_entity* source) override;
private:
	void shoot(physical_entity* source);
	action current_action_ = action::patrol;
	float center_;
	float range_;
	bool direction_;
	std::pair<float, float> angle_range_;
	entity_id close_target_ = false;
	bool can_move_ = true;
	bool retreats_ = false;
	bool obstacle_ = false;
	std::vector<entity_id> targets_;
	time_restriction decision_tick_{ cosmos.universe_clock, 200 };
	time_restriction attack_duration_{ cosmos.universe_clock, 1200 };
	std::uniform_real_distribution<float> bullet_pattern_;
	std::uniform_int_distribution<int> damage_pattern_;
};