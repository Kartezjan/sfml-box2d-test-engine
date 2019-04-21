#include "toolbox_item.h"

void toolbox_item::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(box_, states);
	target.draw(description_, states);
}
