#pragma once
#include <core/module.hpp>

class input_module : public module {
public:
    void init(module_context& ctx)             override;
    void update(module_context& ctx, float dt) override;
    void shutdown(module_context& ctx)         override;
};
