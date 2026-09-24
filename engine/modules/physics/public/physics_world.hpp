#pragma once
#include <core/module.hpp>

class physics_world_module : public module {
public:
    physics_world_module();
    ~physics_world_module();

    void init(module_context& ctx)             override;
    void update(module_context& ctx, float dt) override;
    void shutdown(module_context& ctx)         override;

};
