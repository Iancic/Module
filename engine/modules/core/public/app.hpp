#pragma once
#include <module.hpp>
#include <entt/entt.hpp>
#include <vector>

struct app_running
{
	bool is_running = true;
};

struct module_entry
{
	std::type_info type;
	module module;
};

class app {


	app app{

	}

	void init()
	{

	}

	void run()
	{
		registry.ctx.emplace<app_running>();
	}

private:

	entt::registry registry;
	entt::registry asset_registry;
	std::vector<module_entry> modules;
};