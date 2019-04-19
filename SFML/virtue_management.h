#pragma once

#include "virtue.h"
#include "camera.h"
#include "car.h"
#include "contacts.h"
#include "GUI.h"
#include "misc_virtues.h"
#include "trebuchet.h"
#include "user_input.h"
#include "has_physics.h"

template <typename... Args>
auto make_virtue(const virtue_type type, Args&&... args)
{
	auto result = std::unique_ptr<virtue>{};
	switch(type)
	{
	case virtue_type::empty_virtue:
		break;
	case virtue_type::controllable:
		result = std::make_unique<controllable>{ std::forward(args)... };
		break;
	case virtue_type::controllable_car:
		result = std::make_unique<controllable_car>{ std::forward(args)... };
		break;
	case virtue_type::produces_user_input:
		result = std::make_unique<produces_user_input>{ std::forward(args)... };
		break;
	case virtue_type::applies_force:
		result = std::make_unique<applies_force>{ std::forward(args)... };
		break;
	case virtue_type::spawns_objects:
		result = std::make_unique<spawns_objects>{ std::forward(args)... };
		break;
	case virtue_type::changes_gui_text:
		result = std::make_unique<changes_GUI_text>{ std::forward(args)... };
		break;
	case virtue_type::controls_view:
		result = std::make_unique<controls_view>{ std::forward(args)... };
		break;
	case virtue_type::tracks_object:
		result = std::make_unique<tracks_object>{ std::forward(args)... };
		break;
	case virtue_type::center_of_attention:
		result = std::make_unique<center_of_attention>{ std::forward(args)... };
		break;
	case virtue_type::has_physics:
		result = std::make_unique<has_physics>{ std::forward(args)... };
		break;
	case virtue_type::handles_contacts:
		result = std::make_unique<handles_contacts>{ std::forward(args)... };
		break;
	case virtue_type::destroys_upon_collision:
		result = std::make_unique<destroys_upon_collision>{ std::forward(args)... };
		break;
	case virtue_type::explodes_upon_collision:
		result = std::make_unique<explodes_upon_collision>{ std::forward(args)... };
		break;
	case virtue_type::changes_GUI_text:
		result = std::make_unique<explodes_upon_collision>{ std::forward(args)... };
		break;
	case virtue_type::shows_illusions:
		result = std::make_unique<explodes_upon_collision>{ std::forward(args)... };
		break;
	default:
		assert(false);
	}
	return result;
}
