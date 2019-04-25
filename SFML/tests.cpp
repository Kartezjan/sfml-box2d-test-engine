#include "tests.h"
#include "entity_map.h"
#include "toolbox_item.h"
#include "toolbox.h"
#include "sprite_entity.h"
#include "is_playable.h"
#include "camera.h"

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
	auto test = universe.all_entities += new primitive_entity(create_hero(universe.world, 1500, -250), "foo" ,  universe.resources.textures_["box"]);
	dynamic_cast<sprite_entity*>(universe.all_entities[handle].get())->virtues.push_back(std::make_unique<is_playable>(universe));
	dynamic_cast<sprite_entity*>(universe.all_entities[handle].get())->virtues.push_back(std::make_unique<center_of_attention>(universe));
}

std::pair<std::vector<std::string>, pattern> load_animation_from_file
(
	resource_manager& resources,
	const std::vector<std::pair<std::string,std::string>>& paths,
	std::size_t repeat_per_frame
)
{
	auto new_pattern = pattern{};
	for (auto i = 0; i < paths.size(); ++i)
		for (auto j = 0; j < repeat_per_frame; ++j)
			new_pattern.emplace_back(i);
	auto texture_ids = std::vector<std::string>{};
	for (auto& path : paths) {
		resources.textures_ += path;
		texture_ids.push_back(path.first);
	}
	return { texture_ids, new_pattern };
}

void test_animation(resource_manager& resources, sf::RenderWindow& window)
{
	auto run_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_run0", R"(gfx\fuccboi\adventurer-run-00.png)"},
		{"hero_run1", R"(gfx\fuccboi\adventurer-run-01.png)"},
		{"hero_run2", R"(gfx\fuccboi\adventurer-run-02.png)"},
		{"hero_run3", R"(gfx\fuccboi\adventurer-run-03.png)"},
		{"hero_run4", R"(gfx\fuccboi\adventurer-run-04.png)"},
		{"hero_run5", R"(gfx\fuccboi\adventurer-run-05.png)"}
	};

	auto idle_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_idle0", R"(gfx\fuccboi\adventurer-idle-00.png)"},
		{"hero_idle1", R"(gfx\fuccboi\adventurer-idle-01.png)"},
		{"hero_idle2", R"(gfx\fuccboi\adventurer-idle-02.png)"}
	};
	auto fall_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_fall0", R"(gfx\fuccboi\adventurer-fall-00.png)"},
		{"hero_fall1", R"(gfx\fuccboi\adventurer-fall-01.png)"}
	};
	const auto grab_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_grab0", R"(gfx\fuccboi\adventurer-crnr-grb-00.png)"},
		{"hero_grab1", R"(gfx\fuccboi\adventurer-crnr-grb-01.png)"},
		{"hero_grab2", R"(gfx\fuccboi\adventurer-crnr-grb-02.png)"},
		{"hero_grab3", R"(gfx\fuccboi\adventurer-crnr-grb-03.png)"}
	};
	const auto jump_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_jump0", R"(gfx\fuccboi\adventurer-crnr-jmp-00.png)"},
		{"hero_jump1", R"(gfx\fuccboi\adventurer-crnr-jmp-01.png)"}
	};
	const auto magick_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_magick0", R"(gfx\fuccboi\adventurer-cast-loop-00.png)"},
		{"hero_magick1", R"(gfx\fuccboi\adventurer-cast-loop-01.png)"},
		{"hero_magick2", R"(gfx\fuccboi\adventurer-cast-loop-02.png)"},
		{"hero_magick3", R"(gfx\fuccboi\adventurer-cast-loop-03.png)"},
	};
	auto idle = load_animation_from_file(resources, idle_paths, 15);
	auto run = load_animation_from_file(resources, run_paths, 6);
	auto fall = load_animation_from_file(resources, fall_paths, 6);
	auto grab = load_animation_from_file(resources, grab_paths, 12);
	auto jump = load_animation_from_file(resources, jump_paths, 15);
	auto magick = load_animation_from_file(resources, magick_paths, 6);

	auto& anim_res = resources.anims_res_ += {"hero", idle.first, idle.second};
	resources.anims_res_.update({ "hero" , run.first, run.second });
	resources.anims_res_.update({ "hero" , fall.first, fall.second });
	resources.anims_res_.update({ "hero" , grab.first, grab.second });
	resources.anims_res_.update({ "hero" , jump.first, jump.second });
	resources.anims_res_.update({ "hero" , magick.first, magick.second });
}
