#pragma once

#include <vector>
#include <string>
#include <queue>
#include <unordered_map>
#include <functional>

#include <SFML\Graphics.hpp>
#include <Box2D\Box2D.h>

constexpr auto SCALE = 30.f;

constexpr auto DEG_TO_RAD = 0.01745329251994329576923690768489;
constexpr auto RAD_TO_DEG = (1.0 / 0.01745329251994329576923690768489);
constexpr auto DEG_TO_RADf = 0.01745329251994329576923690768489f;
constexpr auto RAD_TO_DEGf = (1.0f / 0.01745329251994329576923690768489f);

template <typename Vec>
sf::Vector2f to_sf_vec(const Vec& other)
{
	return sf::Vector2f{ other.x, other.y };
}

template <typename Vec>
b2Vec2 to_b2_vec(const Vec& other)
{
	return b2Vec2{ other.x, other.y };
}