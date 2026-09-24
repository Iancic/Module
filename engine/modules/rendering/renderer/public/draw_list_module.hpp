#pragma once
#include <imodule.hpp>
#include <rendering/renderer_backend.hpp>
#include <rendering/mesh_renderer.hpp>
#include <core/transform.hpp>

class DrawListModule : public IModule {
public:
    void init(ModuleContext& ctx)             override;
    void update(ModuleContext& ctx, float dt) override;
    void shutdown(ModuleContext& ctx)         override;

    ModuleAccess access() const override {
        ModuleAccess a;
        a.reads = module_access::ids<MeshRenderer, Transform>();
        return a;
    }

private:
    DrawList m_draw_list;
};
