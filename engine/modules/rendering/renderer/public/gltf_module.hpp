#pragma once
#include <imodule.hpp>
#include <rendering/mesh_loader.hpp>
#include <rendering/render_handle.hpp>
#include <string>
#include <vector>

struct LoadedAsset {
    LoadedScene              scene;
    std::vector<RenderHandle> meshes;
    std::vector<RenderHandle> materials;
};

class GltfModule : public IModule {
public:
    void init(ModuleContext& ctx)             override;
    void update(ModuleContext& ctx, float dt) override;
    void shutdown(ModuleContext& ctx)         override;

    LoadedAsset load(ModuleContext& ctx, const std::string& path);
};
