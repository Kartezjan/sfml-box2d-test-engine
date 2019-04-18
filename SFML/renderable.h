#pragma once

#include"config.h"
#include "virtue.h"

class renderable : public virtue, public sf::Drawable, public sf::Transformable
{
	std::vector < std::unique_ptr<sf::Drawable>> visuals;
private:
	void draw(sf::RenderTarget& target, sf::RenderStates states) const override
	{
		for (auto& vis : visuals)
		{
			states.transform *= getTransform();
			target.draw(*vis, states);
		}
	};
};
