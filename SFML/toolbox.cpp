#include "toolbox.h"
#include "window.h"

void toolbox::add_item(std::string name)
{
	if (name.length() * 8 > item_size_.x)
	{
		item_size_.x = name.length() * 8;
		title_box_size_.x = item_size_.x + toolbox_border_.x;
		item_list_size_.x = item_size_.x;
		title_box_.setSize(title_box_size_);
		for (auto& current : items_)
		{
			current.set_box_size(item_size_);
		}
	}
	const auto item_pos = toolbox_position_ + static_cast<float>(items_.size()) * offset_ + toolbox_border_ / 2.f;
	items_.emplace_back(toolbox_item(item_pos, item_size_, name, font_size_, font_));
	item_list_size_ += offset_;
	update_size();
}

void toolbox::cycle()
{
	items_[current_active_].set_active(false);
	current_active_ = ++current_active_ % items_.size();
	items_[current_active_].set_active(true);
}

void toolbox::select_item(std::size_t i)
{
	items_[current_active_].set_active(false);
	current_active_ = i;
	items_[current_active_].set_active(true);
}

void toolbox::update_size()
{
	item_box_.setSize(item_list_size_ + toolbox_border_);
}

void toolbox::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (!hidden_) {
		target.draw(title_box_, states);
		target.draw(title_, states);
		target.draw(item_box_, states);
		for (auto& item : items_)
		{
			target.draw(item, states);
		}
	}
}

void toolbox::update()
{
	map_to_window(title_box_,title_box_position_, win_ref_);
	map_to_window(title_,title_box_position_, win_ref_);
	map_to_window(item_box_, toolbox_position_, win_ref_);
	for(auto& item : items_)
	{
		const auto position = to_window_cords(item.position(), win_ref_);
		item.update_window_location(position.first);
		item.set_scale(position.second);
	}
}

