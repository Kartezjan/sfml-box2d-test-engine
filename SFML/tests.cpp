#include "tests.h"
#include "ptr_map.h"

void ptr_map_test()
{
	ptr_map<std::string> test;
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
	return image_entity(&anim_res, "test", image_entity::content_type::ANIMATION, window);
}
