#pragma once

#include "config.h"

typedef std::vector<sf::Sprite> animation;
typedef std::vector<size_t> pattern;
struct animation_element
{
public:
	animation_element() = default;
	~animation_element() = default;
	animation_element(const animation_element& other) { animation_ = other.animation_; pattern_ = other.pattern_; }
	animation_element(animation_element&& other) noexcept { animation_ = std::move(other.animation_); pattern_ = std::move(other.pattern_); }
	animation_element(std::vector<sf::Sprite> animation, std::vector<size_t> pattern) : animation_(animation), pattern_(pattern)
	{
		for (auto i : pattern)
			assert(i < animation.size());
	}
	const animation& get_animation() const { return animation_; }
	const pattern& get_pattern() const { return pattern_; }
	sf::Sprite& get_current_frame() { return animation_[pattern_[current_frame]]; }
	const sf::Sprite& get_current_frame() const { return const_cast<animation_element*>(this)->get_current_frame(); }
	void select_frame(size_t frame_id)
	{
		assert(frame_id < pattern_.size());
		current_frame = frame_id;
	}
	void update() { current_frame = ++current_frame % pattern_.size(); }
	void reset() { current_frame = 0; }
private:
	int current_frame = 0;
	animation animation_;
	pattern pattern_;
};

typedef std::vector<animation_element> animation_set;

class animation_resource : public sf::Drawable, public sf::Transformable
{
public:
	animation_resource(animation_set& anims) : anims(anims) {}
	~animation_resource() = default;
	animation_resource(const animation_resource& other)
	{
		operator=(other);
	}
	animation_resource(animation_resource&& other)
	{
		operator=(other);
	}
	animation_resource& operator=(const animation_resource& other)
	{
		std::copy(other.get_animation_set().begin(), other.get_animation_set().end(), std::back_inserter(anims));

		return *this;
	}
	animation_resource& operator=(animation_resource&& other)
	{
		std::move(other.get_animation_set().begin(),other.get_animation_set().end(), std::back_inserter(anims));
		return *this;
	}
	void select_animation(int anim_id)
	{
		assert(anim_id < anims.size());
		current_animation = anim_id;
	}
	const animation_set& get_animation_set() const { return anims; }
	animation_set& get_animation_set() { return anims; }
	animation_element& get_current_animation() { return anims[current_animation]; }
	const animation_element& get_current_animation() const { return const_cast<animation_resource*>(this)->get_current_animation(); }
	void update() { get_current_animation().update(); }
private:
	animation_set anims;
	int current_animation = 0;
	std::vector<size_t> pattern;
	void draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		target.draw(get_current_animation().get_current_frame(), states);
	};
};