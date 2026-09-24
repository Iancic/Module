#pragma once
#include <core/module.hpp>

class window_module : public module {
public:
    window_module();
    ~window_module();

    void init(module_context& ctx)             override;
    void update(module_context& ctx, float dt) override;
    void shutdown(module_context& ctx)         override;
};
