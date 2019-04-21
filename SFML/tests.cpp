#include "tests.h"
#include "entity_map.h"
#include "toolbox_item.h"
#include "toolbox.h"

void test(universe& universe, sf::RenderWindow& window)
{
	auto handle = universe.gui_resources += new toolbox(universe.resources.font, window, { 200.f, 200.f }, "example", "figaro");
	auto foo = dynamic_cast<toolbox*>(universe.gui_resources[handle].get());
	foo->add_item("dupa");
	foo->add_item("test");
	foo->add_item("foo");
	foo->add_item("bar");
}

image_entity test_animation(resource_manager& resources, sf::RenderWindow& window)
{
	auto paths =
	{
		std::make_pair<std::string,std::string>("hero_run0", R"(gfx\fuccboi\run0.png)"),
		std::make_pair<std::string,std::string>("hero_run1", R"(gfx\fuccboi\run1.png)"),
		std::make_pair<std::string,std::string>("hero_run2", R"(gfx\fuccboi\run2.png)"),
		std::make_pair<std::string,std::string>("hero_run3", R"(gfx\fuccboi\run3.png)"),
		std::make_pair<std::string,std::string>("hero_run4", R"(gfx\fuccboi\run4.png)"),
		std::make_pair<std::string,std::string>("hero_run5", R"(gfx\fuccboi\run5.png)")
	};

	auto foo_pattern = pattern{};
	for (auto i = 0; i < paths.size(); ++i)
		for (auto j = 0; j < 6; ++j)
			foo_pattern.emplace_back(i);
	std::vector<std::string> texture_ids;
	for (auto& path : paths) {
		resources.textures_ += path;
		texture_ids.push_back(path.first);
	}
	auto& anim_res = resources.anims_res_ += (std::make_tuple<>("hero_run",texture_ids, foo_pattern));
	return image_entity(&anim_res, { 0,0 }, "test", image_entity::content_type::ANIMATION, window);
}
