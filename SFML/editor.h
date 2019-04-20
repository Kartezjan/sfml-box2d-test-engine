#pragma once

#include <array>

#include "virtue_management.h"

enum class preview_type {box = 0, bomb, count};

class editor : public virtue
{
public:
	editor(universe& uni_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), 
		editor_notifier_("EDITOR MODE", cosmos.resources.font, 22),
		win_ref_(win_ref)
	{
		editor_notifier_.setFillColor(sf::Color::Red);
		const auto pos = sf::Vector2f{static_cast<float>(win_ref.getSize().x - 200), 0};
		notifier_handle = uni_ref.gui_resources += new image_entity(&editor_notifier_, pos, "editor notifier", image_entity::TEXT, win_ref_);
		auto entity = dynamic_cast<image_entity*>(cosmos.gui_resources[notifier_handle].get());
		entity->hidden(hidden_);
		entity->sticky(true);
		make_previews();
		for (auto i = 0; i < previews_data_.size(); ++i)
		{
			const auto handle = uni_ref.gui_resources += new image_entity(previews_data_[i].get(), { 0,0 }, "preview", image_entity::ILLUSION, win_ref_);
			entity = dynamic_cast<image_entity*>(cosmos.gui_resources[handle].get());
			entity->hidden(true);
			previews_[i] = handle;
		}
		current_preview_ = previews_[0];
	}
	void send_message(abstract_entity* source) override;

private:
	void make_previews();
	time_restriction toggle_editor_ {cosmos.universe_clock, 1000};
	sf::Text editor_notifier_;
	entity_id notifier_handle;
	sf::RenderWindow& win_ref_;
	bool hidden_ = true;
	entity_id current_preview_;
	std::array<std::unique_ptr<sf::Drawable>, static_cast<int>(preview_type::count)> previews_data_;
	std::array<entity_id, static_cast<int>(preview_type::count)> previews_;
};
