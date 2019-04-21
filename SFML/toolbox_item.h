#pragma once

#include "entities.h"

class toolbox_item : public sf::Drawable, public sf::Transformable
{
public:
	toolbox_item(
		sf::Vector2f position, 
		sf::Vector2f size,
	//	const sf::Texture& texture,
		const std::string& description,
		std::size_t font_size,
		const sf::Font& font
	) :  description_(description, font, font_size)
	{
		description_.setPosition(position);
		description_.setFillColor(sf::Color::Black);
		//box_.setTexture(&texture);
		set_box_size(size);
		box_.setOutlineThickness(1);
		box_.setOutlineColor(sf::Color::Black);
		box_.setFillColor(passive_color_);
		move(position);
	}
	void change_description(const std::string& new_description) { description_.setString(new_description); }
	void change_font_size(std::size_t size) { description_.setCharacterSize(size); }
	void set_active_color(const sf::Color color) { active_color_ = color; };
	void set_passive_color(const sf::Color color) { passive_color_ = color; }
	void set_outline_color(const sf::Color color) { box_.setOutlineColor(color); }
	bool is_active() const { return is_active_; }
	void set_active(bool opt)
	{
		if(opt)
			box_.setFillColor(active_color_);
		else
			box_.setFillColor(passive_color_);
		is_active_ = opt;
	}
	void move(const sf::Vector2f new_position)
	{
		update_window_location(new_position);
		position_ = new_position;
	}
	void update_window_location(const sf::Vector2f new_position)
	{
		box_.setPosition(new_position);
		description_.setPosition(new_position);
	}
	void set_box_size(const sf::Vector2f border)
	{
		diff_ = border - sf::Vector2f{description_.getLocalBounds().width, description_.getLocalBounds().height};
		// is text in bounds of the box
		assert(diff_.x > 0);
		assert(diff_.y > 0);
		borders_ = border;
		box_.setSize(
			{ 
				border.x, border.y
			});
	}
	sf::Vector2f position() const { return position_; }
	void set_scale(const float scale)
	{
		const auto scale_vec = sf::Vector2f{ scale, scale };
		box_.setScale(scale_vec);
		description_.setScale(scale_vec);
	}
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	sf::Text description_;
	sf::Vector2f position_;
	sf::Vector2f borders_;
	sf::Vector2f diff_;
	sf::RectangleShape box_;
	bool is_active_ = false;
	sf::Color active_color_ = sf::Color(0, 255, 0, 0.9 * 255);
	sf::Color passive_color_ = sf::Color(255,255,255,0.9*255);
};
