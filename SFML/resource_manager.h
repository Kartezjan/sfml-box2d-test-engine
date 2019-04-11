#pragma once

#include "config.h"


struct resource_manager {
	friend resource_manager;
	resource_manager() = default;
	resource_manager(const resource_manager& other)
	{
		operator=(other);
	}
	resource_manager(resource_manager&& other) noexcept
	{
		operator=(other);
	}
	~resource_manager()
	{
		for (auto& e : all_animations_) delete e.second;
		for (auto& e : textures_) delete e.second;
	}
	resource_manager& operator=(const resource_manager& other)
	{
		for(auto& e : other.textures_)
			textures_.emplace(e.first, new sf::Texture(*e.second));
		all_animations_ = other.all_animations_;
		return *this;
	}
	resource_manager& operator=(resource_manager&& other) noexcept
	{
		for (auto& e : textures_) delete e.second;
		textures_.clear();
		for (auto& e : all_animations_) delete e.second;
		all_animations_.clear();
		textures_ = other.textures_;
		all_animations_ = other.all_animations_;
		return *this;
	}
	sf::Texture& add_texture(const std::string& name, const std::string& path)
	{
		auto texture = new sf::Texture();
		assert(texture->loadFromFile(path));
		const auto result = textures_.emplace(name, texture);
		assert(result.second);
		return *result.first->second; 
	}
	sf::Texture& add_texture(const std::pair<std::string, std::string>& description)
	{
		return add_texture(description.first, description.second);
	}
	void remove_texture(const std::string& name)
	{
		auto found = textures_.find(name);
		assert(found != textures_.end());
		delete found->second;
		textures_.erase(found);
	}
	animation_resource& add_animation(const std::string& name, std::vector<std::string>& texture_ids, pattern& pattern)
	{
		auto sprites = animation{};
		for(auto& id : texture_ids)
		{
			sprites.emplace_back(sf::Sprite(get_texture(id)));
			sprites.back().setPosition(sf::Vector2f(300.f,300.f));
		}
		auto anim_res = new animation_resource{ animation_set{animation_element{std::move(sprites), pattern}} };
		const auto result = all_animations_.emplace(name, anim_res);
		assert(result.second);
		return *result.first->second;
	}
	void remove_animation(std::string& name) {
		all_animations_.erase(name);
	}
	sf::Texture& get_texture(const std::string& name)
	{
		const auto found = textures_.find(name);
		assert(found != textures_.end());
		return *found->second;
	}
	animation_resource& get_animation(const std::string& name)
	{
		const auto found = all_animations_.find(name);
		assert(found != all_animations_.end());
		return *found->second;
	}
private:
	std::unordered_map<std::string, sf::Texture*> textures_;
	std::unordered_map<std::string, animation_resource*> all_animations_;
};