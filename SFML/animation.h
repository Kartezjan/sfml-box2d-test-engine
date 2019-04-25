#pragma once

#include "config.h"

typedef std::vector<sf::Texture> animation;
typedef std::vector<size_t> pattern;
struct animation_element
{
public:
	animation_element() = default;
	~animation_element() = default;
	animation_element(const animation_element& other) { animation_ = other.animation_; pattern_ = other.pattern_; }
	animation_element(animation_element&& other) noexcept { animation_ = std::move(other.animation_); pattern_ = std::move(other.pattern_); }
	animation_element(const std::vector<sf::Texture>& animation, std::vector<size_t> pattern) : animation_(animation), pattern_(pattern)
	{
		for (auto i : pattern)
			assert(i < animation.size());
		assert(pattern_.size());
	}
	const animation& get_animation() const { return animation_; }
	const pattern& get_pattern() const { return pattern_; }
	sf::Texture& get_current_frame() { return animation_[pattern_[current_frame]]; }
	const sf::Texture& get_current_frame() const { return const_cast<animation_element*>(this)->get_current_frame(); }
	void select_frame(size_t frame_id)
	{
		assert(frame_id < pattern_.size());
		current_frame = frame_id;
	}
	void update()
	{
		if (repeat_)
			current_frame = ++current_frame % pattern_.size();
		else
			current_frame = std::min(++current_frame, pattern_.size() - 1);
	}
	void reset() { current_frame = 0; }
	void repeats(const bool opt) { repeat_ = opt; }
	bool repeatable() const {return repeat_; }
	bool is_finished() const { return repeat_ ? false : (current_frame == pattern_.size() - 1); }
private:
	std::size_t current_frame = 0;
	animation animation_;
	pattern pattern_;
	bool repeat_ = true;
};

typedef std::vector<animation_element> animation_set;

class animation_resource
{
public:
	explicit animation_resource(animation_set& anims) : anims(anims) {}
	explicit animation_resource() = default;
	~animation_resource() = default;
	animation_resource(const animation_resource& other)
	{
		operator=(other);
	}
	animation_resource(animation_resource&& other) noexcept
	{
		operator=(other);
	}
	animation_resource& operator=(const animation_resource& other)
	{
		std::copy(other.get_animation_set().begin(), other.get_animation_set().end(), std::back_inserter(anims));

		return *this;
	}
	animation_resource& operator=(animation_resource&& other) noexcept
	{
		std::move(other.get_animation_set().begin(), other.get_animation_set().end(), std::back_inserter(anims));
		return *this;
	}
	void operator+=(const animation_element& new_set) { anims.emplace_back(new_set); }
	void select_animation(std::size_t anim_id)
	{
		assert(anim_id < anims.size());
		current_animation = anim_id;
	}
	const animation_set& get_animation_set() const { return anims; }
	animation_set& get_animation_set() { return anims; }
	animation_element& get_current_animation() { return anims[current_animation]; }
	const animation_element& get_current_animation() const { return const_cast<animation_resource*>(this)->get_current_animation(); }
	void update() { get_current_animation().update(); }
	const sf::Texture& get_current_frame() const { return get_current_animation().get_current_frame(); }
	void repeats(const bool opt) { get_current_animation().repeats(opt); }
	bool repeatable() const { return get_current_animation().repeatable(); }
	void reset() { get_current_animation().reset(); }
	bool is_finished() const { return get_current_animation().is_finished(); }
private:
	animation_set anims;
	int current_animation = 0;
	std::vector<size_t> pattern;
};