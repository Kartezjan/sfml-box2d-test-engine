#pragma once

#include <array>

#include "virtue_management.h"

enum class preview_type {box = 0, bomb, count};

constexpr std::size_t box = 0;
constexpr std::size_t bomb = 1;


class editor : public virtue
{
public:
	editor(universe& uni_ref, sf::RenderWindow& win_ref) : virtue(uni_ref), 
		editor_notifier_("EDITOR MODE", cosmos.resources.font, 22),
		win_ref_(win_ref)
	{
		editor_notifier_.setFillColor(sf::Color::Red);
		const auto pos = sf::Vector2f{static_cast<float>(win_ref.getSize().x - 200), 0};
		notifier_handle_ = uni_ref.gui_resources += new image_entity(&editor_notifier_, pos, "editor notifier", image_entity::TEXT, win_ref_);
		auto entity = dynamic_cast<image_entity*>(cosmos.gui_resources[notifier_handle_].get());
		entity->hidden(hidden_);
		entity->sticky(true);
		make_previews();
		make_toolbox();
	}
	void send_message(abstract_entity* source) override;

private:
	void make_previews();
	void make_toolbox();
	void navigate();
	void spawn_primitive();
	void remove_last_spawned_entity();
	auto select_preview(std::size_t i) -> void;

	std::vector<entity_id> spawned_objects_;
	std::size_t current_primitive_;

	time_restriction navigation_{ cosmos.universe_clock, 100 };
	time_restriction spawn_{ cosmos.universe_clock, 300 };
	sf::Text editor_notifier_;
	entity_id notifier_handle_;
	entity_id spawner_gui_handle_;
	entity_id menu_gui_handle_;
	entity_id current_menu_handle_;
	sf::RenderWindow& win_ref_;
	bool hidden_ = true;
	entity_id current_preview_;
	std::array<std::unique_ptr<sf::Drawable>, static_cast<int>(preview_type::count)> previews_data_;
	std::array<entity_id, static_cast<int>(preview_type::count)> previews_;
};
