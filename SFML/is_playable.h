#pragma once
#include "virtue_management.h"
#include "scanner.h"

class sprite_entity;

class is_playable : public virtue
{
public:
	is_playable(universe& uni_ref) :  virtue(uni_ref), 
	scanner_(uni_ref, 5.f * DEG_TO_RADf, 500 / SCALE) {}
	enum class state { idle = 0, running };
	void send_message(abstract_entity* source) override;
private:
	state current_state_ = state::idle;
	time_restriction jump_cooldown_{cosmos.universe_clock, 500};
	time_restriction magick_cooldown_{cosmos.universe_clock, 3000};
	time_restriction magick_cast_duration_{cosmos.universe_clock, 250};
	bool casts_magick_ = false;
	bool stands_on_ground(const entity_id source) const;
	void die(sprite_entity* who);
	bool inv_direction_ = false;
	bool died_ = false;
	scanner scanner_;
};
