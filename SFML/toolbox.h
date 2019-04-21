#pragma once

#include  "entities.h"
#include  "toolbox_item.h"


class toolbox : public renderable_entity
{
public:
	toolbox
	(
		const sf::Font& font,
		const sf::RenderWindow& win_ref,
		const sf::Vector2f title_position,
		const std::string& title_name,
		const std::string& first_item_name)
		: font_(font), win_ref_(win_ref), title_box_position_(title_position), title_(title_name, font, 14)
	{
		title_.setFillColor(sf::Color::Black);
		title_.setPosition(title_position);
		add_item(first_item_name);
		items_.back().set_active(true);
		item_box_.setPosition(toolbox_position_);
		item_box_.setFillColor(sf::Color(51, 102, 204, 0.6*255));
		title_box_.setFillColor(sf::Color(0,0,255,0.6*255));
		title_box_.setSize(title_box_size_);
		title_box_.setPosition(title_box_position_);

	}
	void update() override;
	void add_item(std::string name);
	void cycle();
	void select_item(std::size_t i);
	std::size_t get_current_active() const { return current_active_; }
	void update_size();
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::size_t font_size_ = 12;
	sf::Vector2f item_size_ = {100, 20};
	sf::Vector2f offset_ = {0, item_size_.y + 5.f};
	std::vector<toolbox_item> items_;
	std::size_t current_active_;
	sf::Vector2f title_box_position_;
	sf::Vector2f toolbox_border_ = { 10, 20 };
	sf::Vector2f title_box_size_ = { item_size_.x + toolbox_border_.x, 20 };
	sf::Vector2f toolbox_position_ = title_box_position_ + sf::Vector2f{ 0, title_box_size_.y };
	sf::Vector2f item_list_size_ = { item_size_.x, 0 };

	sf::Text title_;
	sf::RectangleShape item_box_;
	sf::RectangleShape title_box_;
	const sf::Font& font_;
	const sf::RenderWindow& win_ref_;
};
