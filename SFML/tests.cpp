#include "tests.h"
#include "entity_map.h"
#include "toolbox_item.h"
#include "toolbox.h"
#include "sprite_entity.h"
#include "is_playable.h"
#include "camera.h"
#include "behavior.h"

void gui_test(universe& universe, sf::RenderWindow& window)
{
	auto handle = universe.gui_resources += new toolbox(universe.resources.font, window, { 200.f, 200.f }, "example", "figaro");
	auto foo = dynamic_cast<toolbox*>(universe.gui_resources[handle].get());
	foo->add_item("dupa");
	foo->add_item("test");
	foo->add_item("foo");
	foo->add_item("bar");
}

entity_id hero_test(universe& universe)
{
	auto handle = universe.all_entities += new sprite_entity(create_hero(universe.world, 1000, -250), universe.resources.anims_res_["hero"], 3.f);
	auto test = universe.all_entities += new primitive_entity(create_hero(universe.world, 1500, -250), "foo" ,  universe.resources.textures_["box"]);
	dynamic_cast<sprite_entity*>(universe.all_entities[handle].get())->virtues.push_back(std::make_unique<is_playable>(universe));
	dynamic_cast<sprite_entity*>(universe.all_entities[handle].get())->virtues.push_back(std::make_unique<center_of_attention>(universe));
	dynamic_cast<sprite_entity*>(universe.all_entities[handle].get())->set_category(sprite_entity::category::enemy);

	auto ranger_handle = universe.all_entities += new sprite_entity(create_ranger(universe.world, 1970, -250), universe.resources.anims_res_["ranger"], 3.f);
	dynamic_cast<sprite_entity*>(universe.all_entities[ranger_handle].get())->virtues.push_back(
		std::make_unique<ranger_behavior>(ranger_behavior{universe, 1970.f, 300.f, {-15.f * DEG_TO_RADf, 15.f * DEG_TO_RADf}, true}));
	dynamic_cast<sprite_entity*>(universe.all_entities[ranger_handle].get())->set_category(sprite_entity::category::ally);

	return handle;
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

void load_ranger(resource_manager& resources)
{
	auto attack_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"ranger_attack0", R"(gfx\ranger\Attack_02\Character_01_Attack_02_01.png)"},
		{"ranger_attack1", R"(gfx\ranger\Attack_02\Character_01_Attack_02_02.png)"},
		{"ranger_attack2", R"(gfx\ranger\Attack_02\Character_01_Attack_02_03.png)"},
		{"ranger_attack3", R"(gfx\ranger\Attack_02\Character_01_Attack_02_04.png)"},
		{"ranger_attack4", R"(gfx\ranger\Attack_02\Character_01_Attack_02_05.png)"},
		{"ranger_attack5", R"(gfx\ranger\Attack_02\Character_01_Attack_02_06.png)"},
		{"ranger_attack6", R"(gfx\ranger\Attack_02\Character_01_Attack_02_07.png)"},
		{"ranger_attack7", R"(gfx\ranger\Attack_02\Character_01_Attack_02_08.png)"},
		{"ranger_attack8", R"(gfx\ranger\Attack_02\Character_01_Attack_02_09.png)"},
		{"ranger_attack9", R"(gfx\ranger\Attack_02\Character_01_Attack_02_10.png)"},
		{"ranger_attack10", R"(gfx\ranger\Attack_02\Character_01_Attack_02_11.png)"},
		{"ranger_attack11", R"(gfx\ranger\Attack_02\Character_01_Attack_02_12.png)"},
		{"ranger_attack12", R"(gfx\ranger\Attack_02\Character_01_Attack_02_13.png)"},
		{"ranger_attack13", R"(gfx\ranger\Attack_02\Character_01_Attack_02_14.png)"},
		{"ranger_attack14", R"(gfx\ranger\Attack_02\Character_01_Attack_02_15.png)"},
		{"ranger_attack15", R"(gfx\ranger\Attack_02\Character_01_Attack_02_16.png)"}
	};

	auto walk_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"ranger_walk0", R"(gfx\ranger\Walk\Character_01_Walk_01.png)"},
		{"ranger_walk1", R"(gfx\ranger\Walk\Character_01_Walk_02.png)"},
		{"ranger_walk2", R"(gfx\ranger\Walk\Character_01_Walk_03.png)"},
		{"ranger_walk3", R"(gfx\ranger\Walk\Character_01_Walk_04.png)"},
		{"ranger_walk4", R"(gfx\ranger\Walk\Character_01_Walk_05.png)"},
		{"ranger_walk5", R"(gfx\ranger\Walk\Character_01_Walk_06.png)"},
		{"ranger_walk6", R"(gfx\ranger\Walk\Character_01_Walk_07.png)"},
		{"ranger_walk7", R"(gfx\ranger\Walk\Character_01_Walk_08.png)"},
		{"ranger_walk8", R"(gfx\ranger\Walk\Character_01_Walk_09.png)"},
		{"ranger_walk9", R"(gfx\ranger\Walk\Character_01_Walk_10.png)"},
		{"ranger_walk10", R"(gfx\ranger\Walk\Character_01_Walk_11.png)"},
		{"ranger_walk11", R"(gfx\ranger\Walk\Character_01_Walk_12.png)"}
	};

	auto idle_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"ranger_idle0", R"(gfx\ranger\Idle\Character_01_Idle1.png)"},
		{"ranger_idle1", R"(gfx\ranger\Idle\Character_01_Idle2.png)"},
		{"ranger_idle2", R"(gfx\ranger\Idle\Character_01_Idle3.png)"},
		{"ranger_idle3", R"(gfx\ranger\Idle\Character_01_Idle4.png)"},
		{"ranger_idle4", R"(gfx\ranger\Idle\Character_01_Idle5.png)"},
		{"ranger_idle5", R"(gfx\ranger\Idle\Character_01_Idle6.png)"},
		{"ranger_idle6", R"(gfx\ranger\Idle\Character_01_Idle7.png)"},
		{"ranger_idle7", R"(gfx\ranger\Idle\Character_01_Idle8.png)"},
		{"ranger_idle8", R"(gfx\ranger\Idle\Character_01_Idle9.png)"},
		{"ranger_idle9", R"(gfx\ranger\Idle\Character_01_Idle10.png)"},
		{"ranger_idle10", R"(gfx\ranger\Idle\Character_01_Idle11.png)"},
		{"ranger_idle11", R"(gfx\ranger\Idle\Character_01_Idle12.png)"}
	};
	auto death_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"ranger_death0", R"(gfx\ranger\Death\Character_01_Death_01.png)"},
		{"ranger_death1", R"(gfx\ranger\Death\Character_01_Death_02.png)"},
		{"ranger_death2", R"(gfx\ranger\Death\Character_01_Death_03.png)"},
		{"ranger_death3", R"(gfx\ranger\Death\Character_01_Death_04.png)"},
		{"ranger_death4", R"(gfx\ranger\Death\Character_01_Death_05.png)"},
		{"ranger_death5", R"(gfx\ranger\Death\Character_01_Death_06.png)"},
		{"ranger_death6", R"(gfx\ranger\Death\Character_01_Death_07.png)"},
		{"ranger_death7", R"(gfx\ranger\Death\Character_01_Death_08.png)"},
		{"ranger_death8", R"(gfx\ranger\Death\Character_01_Death_09.png)"},
		{"ranger_death9", R"(gfx\ranger\Death\Character_01_Death_10.png)"},
		{"ranger_death10", R"(gfx\ranger\Death\Character_01_Death_11.png)"},
		{"ranger_death11", R"(gfx\ranger\Death\Character_01_Death_12.png)"},
		{"ranger_death12", R"(gfx\ranger\Death\Character_01_Death_13.png)"},
		{"ranger_death13", R"(gfx\ranger\Death\Character_01_Death_14.png)"},
		{"ranger_death14", R"(gfx\ranger\Death\Character_01_Death_15.png)"},
		{"ranger_death15", R"(gfx\ranger\Death\Character_01_Death_16.png)"},
	};
	auto idle = load_animation_from_file(resources, idle_paths, 6);
	auto walk = load_animation_from_file(resources, walk_paths, 6);
	auto attack = load_animation_from_file(resources, attack_paths, 6);
	auto death = load_animation_from_file(resources, death_paths, 6);
	auto& anim_res = resources.anims_res_ += {"ranger", idle.first, idle.second};
	resources.anims_res_.update({ "ranger", walk.first, walk.second });
	resources.anims_res_.update({ "ranger", attack.first, attack.second });
	resources.anims_res_.update({ "ranger", death.first, death.second });
}

void load_hero(resource_manager& resources)
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
	const auto death_paths = std::vector<std::pair<std::string, std::string>>
	{
		{"hero_death1", R"(gfx\fuccboi\adventurer-die-01.png)"},
		{"hero_death2", R"(gfx\fuccboi\adventurer-die-02.png)"},
		{"hero_death3", R"(gfx\fuccboi\adventurer-die-03.png)"},
		{"hero_death4", R"(gfx\fuccboi\adventurer-die-04.png)"},
		{"hero_death5", R"(gfx\fuccboi\adventurer-die-05.png)"},
		{"hero_death6", R"(gfx\fuccboi\adventurer-die-06.png)"},
	};
	auto idle = load_animation_from_file(resources, idle_paths, 15);
	auto run = load_animation_from_file(resources, run_paths, 6);
	auto fall = load_animation_from_file(resources, fall_paths, 6);
	auto grab = load_animation_from_file(resources, grab_paths, 12);
	auto jump = load_animation_from_file(resources, jump_paths, 15);
	auto magick = load_animation_from_file(resources, magick_paths, 6);
	auto death = load_animation_from_file(resources, death_paths, 30);

	auto& anim_res = resources.anims_res_ += {"hero", idle.first, idle.second};
	resources.anims_res_.update({ "hero" , run.first, run.second });
	resources.anims_res_.update({ "hero" , fall.first, fall.second });
	resources.anims_res_.update({ "hero" , grab.first, grab.second });
	resources.anims_res_.update({ "hero" , jump.first, jump.second });
	resources.anims_res_.update({ "hero" , magick.first, magick.second });
	resources.anims_res_.update({ "hero" , death.first, death.second });
}

void test_animation(resource_manager& resources, sf::RenderWindow& window)
{
	load_hero(resources);
	load_ranger(resources);
}
