#include "editor.h"

void editor::send_message(abstract_entity* source)
{
	auto& input = cosmos.message_queues.get_queue<input_message>();
	for(auto& msg : input)
	{
		if(msg.key == input_key::F1)
		{
			if (toggle_editor_.can_use())
			{
				auto current = dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get());
				cosmos.editor_mode = !cosmos.editor_mode;
				hidden_ = !hidden_;
				dynamic_cast<image_entity*>(cosmos.gui_resources[notifier_handle].get())->hidden(hidden_);
				if(cosmos.editor_mode)
				{
					current->hidden(false);
				}
				else
				{
					current->hidden(true);
				}
			}
			msg.delete_this_message = true;
		}
	}
	if(cosmos.editor_mode)
	{
		auto current = dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get());
		current->update_position(cosmos.mouse_pos);
	}

}

void editor::make_previews()
{
	auto box_shape = utillity::convert_shape_to_sf
	(
		create_box(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y), cosmos.resources.textures_["box"]
	);
	auto bomb_shape = utillity::convert_shape_to_sf
	(
		create_circle(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y, 16, 1.f, 1.f), cosmos.resources.textures_["black"]
	);
	assert(box_shape.size() == 1);
	assert(bomb_shape.size() == 1);
	// make it green
	dynamic_cast<sf::ConvexShape*>(box_shape.back().visual_object)->setFillColor(sf::Color::Green);
	dynamic_cast<sf::CircleShape*>(bomb_shape.back().visual_object)->setFillColor(sf::Color::Green);

	previews_data_[static_cast<int>(preview_type::box)] = std::unique_ptr<sf::Drawable>(box_shape.back().visual_object);
	previews_data_[static_cast<int>(preview_type::bomb)] = std::unique_ptr<sf::Drawable>(bomb_shape.back().visual_object);
}
