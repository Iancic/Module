#pragma once
#include <core/module.hpp>

class audio_module : public module {
public:
    audio_module();
    ~audio_module() override;

    void init(ModuleContext& ctx)             override;
    void update(ModuleContext& ctx, float dt) override;
    void shutdown(ModuleContext& ctx)         override;

};
