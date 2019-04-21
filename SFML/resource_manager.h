#pragma once

#include "config.h"
#include "animation.h"

typedef std::vector<std::string> texture_ids;
typedef std::string resource_id;
typedef std::string path;

template<typename T>
class resource
{
public:
	friend resource;
	resource() = default;
	resource(const resource& other) { operator=(other); }
	resource(resource&& other) noexcept { operator=(other); }
	~resource() { for (auto& e : resource_) delete e.second; }
	resource& operator=(const resource& other)
	{
		for(auto& e : other.resource_)
			resource_.emplace(e.first, new T(*e.second));
		return *this;
	}
	resource& operator=(resource&& other) noexcept
	{
		for (auto& e : resource_) delete e.second;
		resource_.clear();
		resource_ = other.resource_;
		return *this;
	}
	T& operator[](const resource_id& idx){
		const auto found = resource_.find(idx);
		assert(found != resource_.end());
		return *found->second;
	}

	void operator-=(const resource_id& idx)
	{
		auto found = resource_.find(idx);
		assert(found != resource_.end());
		delete found->second;
		resource_.erase(found);
	}
	const T& operator[] (const resource_id& idx) const{ return const_cast<resource<T>*>(this)->operator[](idx); }
protected:
	std::unordered_map<std::string, T*> resource_;
};

class textures : public resource<sf::Texture>
{
public:
	sf::Texture& add(const resource_id& id, const path& path) { return operator+=(std::make_pair<>(id, path)); }
	sf::Texture& operator+=(const std::pair<resource_id, path>& description)
	{
		auto texture = new sf::Texture();
		assert(texture->loadFromFile(description.second));
		const auto result = resource_.emplace(description.first, texture);
		assert(result.second);
		return *result.first->second; 
	}
};
class animation_resources : public resource<animation_resource>
{
public:
	animation_resources(const textures& textures) : textures_ref(textures) {}
	animation_resource& add(resource_id id, texture_ids textures, pattern pattern)
	{
		return operator+=(std::make_tuple<>(id, textures, pattern));
	}
	void update(const std::tuple<resource_id, texture_ids, pattern>& description)
	{
		auto& anim_id = std::get<0>(description);
		assert(resource_.find(anim_id) != resource_.end());
		auto& textures = std::get<1>(description);
		auto& pattern = std::get<2>(description);
		auto sprites = animation{};
		for(auto& id : textures)
		{
			sprites.emplace_back(sf::Sprite(textures_ref[id]));
		}
		*resource_[anim_id] += animation_element{ sprites, pattern };
	}
	animation_resource& operator+=(const std::tuple<resource_id, texture_ids, pattern>& description)
	{
		auto& anim_id = std::get<0>(description);
		auto anim_res = new animation_resource();
		const auto result = resource_.emplace(anim_id, anim_res);
		update(description);
		assert(result.second);
		return *result.first->second;
	}
private:
	const textures& textures_ref;
};


struct resource_manager {
	resource_manager() : anims_res_(textures_) {}
	textures textures_;
	animation_resources anims_res_;
	sf::Font font;
};