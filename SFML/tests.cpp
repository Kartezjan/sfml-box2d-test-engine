#include "tests.h"
#include "entity_map.h"
#include "toolbox_item.h"
#include "toolbox.h"
#include "sprite_entity.h"

void gui_test(universe& universe, sf::RenderWindow& window)
{
	auto handle = universe.gui_resources += new toolbox(universe.resources.font, window, { 200.f, 200.f }, "example", "figaro");
	auto foo = dynamic_cast<toolbox*>(universe.gui_resources[handle].get());
	foo->add_item("dupa");
	foo->add_item("test");
	foo->add_item("foo");
	foo->add_item("bar");
}

void hero_test(universe& universe)
{
	auto handle = universe.all_entities += new sprite_entity(create_hero(universe.world, 1000, -250), universe.resources.anims_res_["hero"]);
	dynamic_cast<sprite_entity*>(universe.all_entities[handle].get())->virtues.push_back(std::make_unique<controllable>(universe));
}

void test_animation(resource_manager& resources, sf::RenderWindow& window)
{
	auto paths = std::vector<std::pair<std::string,std::string>>
	{
		{"hero_run0", R"(gfx\fuccboi\adventurer-run-00.png)"},
		{"hero_run1", R"(gfx\fuccboi\adventurer-run-01.png)"},
		{"hero_run2", R"(gfx\fuccboi\adventurer-run-02.png)"},
		{"hero_run3", R"(gfx\fuccboi\adventurer-run-03.png)"},
		{"hero_run4", R"(gfx\fuccboi\adventurer-run-04.png)"},
		{"hero_run5", R"(gfx\fuccboi\adventurer-run-05.png)"}
	};

	auto idle_paths = std::vector<std::pair<std::string,std::string>>
	{
		{"hero_idle0", R"(gfx\fuccboi\adventurer-idle-00.png)"},
		{"hero_idle1", R"(gfx\fuccboi\adventurer-idle-01.png)"},
		{"hero_idle2", R"(gfx\fuccboi\adventurer-idle-02.png)"}
	};

	auto run_pattern = pattern{};
	for (auto i = 0; i < paths.size(); ++i)
		for (auto j = 0; j < 6; ++j)
			run_pattern.emplace_back(i);

	std::vector<std::string> texture_run_ids;
	for (auto& path : paths) {
		resources.textures_ += path;
		texture_run_ids.push_back(path.first);
	}

	std::vector<std::string> texture_idle_ids;
	for (auto& path : idle_paths) {
	resources.textures_ += path;
	texture_idle_ids.push_back(path.first);
	}

	auto idle_pattern = pattern{};
	for (auto i = 0; i < idle_paths.size(); ++i)
		for (auto j = 0; j < 6; ++j)
			idle_pattern.emplace_back(i);
	auto& anim_res = resources.anims_res_ += {"hero", texture_run_ids, run_pattern};
	resources.anims_res_.update({ "hero" , texture_idle_ids, idle_pattern });
}
