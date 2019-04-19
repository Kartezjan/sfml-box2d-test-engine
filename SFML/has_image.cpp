#include "has_image.h"
#include "renderable.h"

void has_image::process()
{

}

auto has_image::init_render() const -> visual_container& 
{
	const auto result = owner_ref_.add_virtue(virtue_type::renderable);
	const auto renderable_virtue = dynamic_cast<renderable*>(result.first.get());
	return renderable_virtue->make_container();
}
