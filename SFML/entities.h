#pragma once

#include "config.h"
#include "utillity.h"
#include "animation.h"
#include "virtue_management.h"
#include "physics.h"
#include "ptr_vec.h"

class virtue;
using entity_id = std::size_t;

enum class entity_type {ABSTRACT, IMAGE, PHYSICAL, OTHER};
enum class visual_effect_type { ROPE_JOINT };

struct visual_effect {
	visual_effect_type type;
	sf::Drawable* visual_object;
	float property;
};

class abstract_entity {
public:
	virtual ~abstract_entity() = default;
	abstract_entity(const entity_id id = 0) : id_(id) {}
	std::string name;
	std::vector<std::unique_ptr<virtue>> virtues;
	entity_type get_type() { return type; }
	const std::string& get_name() { return name; }
	virtual void spec() {}
	entity_id id() const { return id_; }
	void change_id(const entity_id id) { id_ = id; }
protected:
	entity_type type = entity_type::ABSTRACT;
	entity_id id_;
};

class renderable_entity : public abstract_entity, public sf::Drawable, public sf::Transformable {
public:
	renderable_entity(const entity_id id = 0) : abstract_entity(id) {}
	virtual void update(void) = 0;
	bool is_hidden() const { return hidden_; }
	void hidden(const bool opt) { hidden_ = opt; }
protected:
	bool hidden_ = false;
private:
	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const = 0;
};

class image_entity : public renderable_entity {
public:
	enum content_type { MOUSE_CORD_MESSAGE, TEXT, ILLUSION, ROPE, ANIMATION };
	image_entity(sf::Drawable* object, const sf::Vector2f position, const std::string& n_name, const content_type n_type, sf::RenderWindow& win_ref);
	void update(void) override;
	sf::Drawable* get_visual_object() const { return visual_object.get(); }
	content_type get_image_type() const { return image_type; };
	bool is_sticky() const { return sticky_; }
	void sticky(const bool opt) { sticky_ = opt; }
	void update_position(const sf::Vector2f new_pos) { position_ = new_pos; }
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	std::unique_ptr<sf::Drawable> visual_object;
	content_type image_type;
	sf::RenderWindow& window;
	bool sticky_ = false;
	sf::Vector2f position_;
};

class physical_entity : public renderable_entity 
{
public:
	physical_entity(entity_id id = 0) : renderable_entity(id) {}
	~physical_entity()
	{
		for(auto fixture = physical_body->GetFixtureList(); fixture; fixture = fixture->GetNext())
		{
			if(const auto ptr = static_cast<fixture_data*>(fixture->GetUserData()))
				delete ptr;
		}
		physical_body->GetWorld()->DestroyBody(physical_body);
	}
	b2Body* get_physical_body() { return physical_body; };
	void update() override { assert(false); }
protected:
	b2Body* physical_body = nullptr; 
	void draw(sf::RenderTarget& target, sf::RenderStates states) const { assert(false); }
};

class primitive_entity : public physical_entity {
public:
	void addVisualEffect(visual_effect effect) { visual_effects.push_back(effect); };
	primitive_entity(const body_properties& body_properties, std::string n_name, const sf::Texture& box_texture);
	~primitive_entity();
	void expand_texture_rect();
	void update() override;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override;
	const sf::Texture& texture;
	std::vector<shape> shapes;
	std::vector<visual_effect> visual_effects;
};


