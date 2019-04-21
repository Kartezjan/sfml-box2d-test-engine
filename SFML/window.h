#pragma once

#include "config.h"

inline void map_to_window(sf::Transformable& transformable, const sf::Vector2f position, const sf::RenderWindow& window)
{
	const auto pos = sf::Vector2i{ static_cast<const int>(position.x), static_cast<const int>(position.y) };
	transformable.setPosition(window.mapPixelToCoords(pos));
	const auto scale = window.getView().getSize().x / window.getSize().x;
	transformable.setScale(sf::Vector2f(scale, scale));
}

inline void map_to_window(sf::Transformable& transformable, const sf::RenderWindow& window)
{
	const auto pos = sf::Vector2i{ static_cast<const int>(transformable.getPosition().x)
		, static_cast<const int>(transformable.getPosition().y) };
	transformable.setPosition(window.mapPixelToCoords(pos));
	const auto scale = window.getView().getSize().x / window.getSize().x;
	transformable.setScale(sf::Vector2f(scale, scale));
}

inline std::pair<sf::Vector2f, float> to_window_cords(const sf::Vector2f position, const sf::RenderWindow& window){
	
	const auto pos = sf::Vector2i{ static_cast<const int>(position.x), static_cast<const int>(position.y) };
	return { window.mapPixelToCoords(pos), window.getView().getSize().x / window.getSize().x };
}
