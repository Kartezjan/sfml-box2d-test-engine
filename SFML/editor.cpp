#include "editor.h"
#include "toolbox.h"
#include "contacts.h"

void editor::send_message(abstract_entity* source)
{
	auto& input = cosmos.message_queues.get_queue<input_message>();
	for (auto& msg : input)
	{
		switch (msg.key)
		{
		case input_key::F1:
			if (navigation_.can_use())
			{
				cosmos.editor_mode = !cosmos.editor_mode;
				auto current = dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get());
				current->hidden(!(cosmos.editor_mode && current_menu_handle_ == spawner_gui_handle_));
				hidden_ = !hidden_;
				dynamic_cast<image_entity*>(cosmos.gui_resources[notifier_handle_].get())->hidden(hidden_);
				dynamic_cast<toolbox*>(cosmos.gui_resources[current_menu_handle_].get())->hidden(hidden_);
			}
			msg.delete_this_message = true;
			break;
		case input_key::Period:
			if (cosmos.editor_mode && navigation_.can_use())
			{
				dynamic_cast<toolbox*>(cosmos.gui_resources[current_menu_handle_].get())->cycle();
			}
			msg.delete_this_message = true;
			break;
		case input_key::Enter:
			if (cosmos.editor_mode && navigation_.can_use())
			{
				navigate();
			}
			msg.delete_this_message = true;
			break;
		case input_key::LMB:
			if(cosmos.editor_mode && current_menu_handle_ == spawner_gui_handle_ && spawn_.can_use())
			{
				spawn_primitive();
			}
			msg.delete_this_message = true;
			break;
		case input_key::RMB:
			if(cosmos.editor_mode && current_menu_handle_ == spawner_gui_handle_ && spawn_.can_use())
			{
				remove_last_spawned_entity();
			}
			msg.delete_this_message = true;
			break;
		default:
			break;
		}
	}
	if (cosmos.editor_mode)
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

	previews_data_[box] = std::unique_ptr<sf::Drawable>(box_shape.back().visual_object);
	previews_data_[bomb] = std::unique_ptr<sf::Drawable>(bomb_shape.back().visual_object);
	for (auto i = 0; i < previews_data_.size(); ++i)
	{
		const auto handle = cosmos.gui_resources += new image_entity(previews_data_[i].get(), { 0,0 }, "preview", image_entity::ILLUSION, win_ref_);
		auto entity = dynamic_cast<image_entity*>(cosmos.gui_resources[handle].get());
		entity->hidden(true);
		previews_[i] = handle;
	}
	current_preview_ = previews_[0];
}

void editor::make_toolbox()
{
	auto toolbox_pos = sf::Vector2f{ static_cast<float>(win_ref_.getSize().x - 180), 30 };
	spawner_gui_handle_ = cosmos.gui_resources += new toolbox
	(
		cosmos.resources.font, win_ref_, toolbox_pos, "Spawn Objects", "Box"

	);
	const auto spawner = dynamic_cast<toolbox*>(cosmos.gui_resources[spawner_gui_handle_].get());
	spawner->add_item("Bomb");
	spawner->add_item("Back...");
	spawner->hidden(true);

	menu_gui_handle_ = cosmos.gui_resources += new toolbox
	(
		cosmos.resources.font, win_ref_, toolbox_pos, "Editor Menu", "Spawn Objects"
	);

	const auto menu = dynamic_cast<toolbox*>(cosmos.gui_resources[menu_gui_handle_].get());
	menu->add_item("Create Ground");
	menu->hidden(true);
	current_menu_handle_ = menu_gui_handle_;
}

void editor::navigate()
{
	const auto current_menu = dynamic_cast<toolbox*>(cosmos.gui_resources[current_menu_handle_].get());
	const auto selected = current_menu->get_current_active();
	if(current_menu_handle_ == menu_gui_handle_)
	{
		switch(selected)
		{
		case 0: // Spawner
			current_menu->hidden(true);
			dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get())->hidden(false);
			current_menu_handle_ = spawner_gui_handle_;
			dynamic_cast<toolbox*>(cosmos.gui_resources[spawner_gui_handle_].get())->hidden(false);
			dynamic_cast<toolbox*>(cosmos.gui_resources[spawner_gui_handle_].get())->select_item(0);
		default:
			break;
		}

	}
	else if (current_menu_handle_ == spawner_gui_handle_)
	{
		switch(selected)
		{
		case box:
			select_preview(box);
			current_primitive_ = box;
			break;
		case bomb:
			select_preview(bomb);
			current_primitive_ = bomb;
			break;
		default: //exit
			current_menu->hidden(true);
			dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get())->hidden(true);
			current_menu_handle_ = menu_gui_handle_;
			dynamic_cast<toolbox*>(cosmos.gui_resources[menu_gui_handle_].get())->hidden(false);
			break;
		}
	}
}

void editor::spawn_primitive()
{
	auto handle = 0;
	primitive_entity* obj = nullptr;
	switch(current_primitive_)
	{
	case box:
		handle = cosmos.all_entities += new primitive_entity
		(
			create_box(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y),
			"box",
			cosmos.resources.textures_["box"]
		);
		break;
	case bomb:
		handle = cosmos.all_entities += new primitive_entity
		(
			create_circle(cosmos.world, cosmos.mouse_pos.x, cosmos.mouse_pos.y, 16, 1.f, 1.f), 
			"bomb",
			cosmos.resources.textures_["black"]
		);
		obj = dynamic_cast<primitive_entity*>(cosmos.all_entities.access(handle).get());
		obj->virtues.push_back(std::make_unique<destroys_upon_collision>(cosmos));
		obj->virtues.push_back(std::make_unique<explodes_upon_collision>(cosmos, 200.f, 1e+5F ));
		break;
	default:
		assert(false);
		break;
	}
	spawned_objects_.push_back(handle);
}

void editor::remove_last_spawned_entity()
{
	if (!spawned_objects_.empty()) 
	{
		const entity_id obj_to_del_handle = spawned_objects_.back();
		auto& death_queue = cosmos.message_queues.get_queue<death_message>();
		death_message msg;
		msg.target = obj_to_del_handle;
		death_queue.emplace_back(msg);
		spawned_objects_.pop_back();
	}
}

void editor::select_preview(std::size_t i)
{
	dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get())->hidden(true);
	current_preview_ = previews_[i];
	dynamic_cast<image_entity*>(cosmos.gui_resources[current_preview_].get())->hidden(false);
}

