-- Platforms
local has_prospero = os.isfile("platform/prospero/prospero.lua")
has_windows = os.isdir("platform/windows")

-- Vendor
Vendor = {}

if has_windows then
    Vendor["SDL3"] = {
        includedirs = { "platform/windows/vendor/SDL3/include" },
        libdirs     = { "platform/windows/vendor/SDL3/lib/x64" },
        links       = { "SDL3" },
    }
end

Vendor["FMOD"] = { includedirs = { "vendor/fmod" } }

Vendor["Jolt"]         = { includedirs = { "vendor" } }
Vendor["glm"]          = { includedirs = { "vendor" } }
Vendor["fmt"]          = { includedirs = { "vendor/fmt/include" } }
Vendor["EnTT"]         = { includedirs = { "vendor/entt/include" } }
Vendor["magic_enum"]   = { includedirs = { "vendor/magic_enum" } }
Vendor["mikktspace"]   = { includedirs = { "vendor/mikktspace" } }
Vendor["tinygltf"]     = { includedirs = { "vendor/tinygltf" } }
Vendor["ufbx"]         = { includedirs = { "vendor/ufbx" } }
Vendor["visit_struct"] = { includedirs = { "vendor/visit_struct" } }

local function resolveVendorPaths(dirs)
    local resolved = {}
    for _, dir in ipairs(dirs) do
        table.insert(resolved, path.join(_MAIN_SCRIPT_DIR, dir))
    end
    return resolved
end

function UseVendor(name)
    local vendor = Vendor[name]
    if not vendor then
        error("UseVendor: unknown vendor '" .. name .. "'")
    end
    if vendor.includedirs then
        includedirs(resolveVendorPaths(vendor.includedirs))
    end
    if vendor.libdirs then
        libdirs(resolveVendorPaths(vendor.libdirs))
    end
    if vendor.links then
        links(vendor.links)
    end
end

-- Clang tooling
include "tools/clang/clang_premake.lua"

-- Workspace
workspace "Module"
    configurations {"debug", "release", "development"}
    platforms {"windows"}
    architecture "x86_64"
    startproject "Module"
    location "build/vs"

    filter "configurations:debug"
        defines { "DEBUG" }
        symbols "On"

    filter "configurations:development"
        defines { "DEVELOPMENT" }
        symbols "On"
        optimize "On"

    filter "configurations:release"
        defines { "RELEASE" }
        symbols "Off"
        optimize "On"

    if has_prospero then
        platforms{"prospero"}
    end

-- Modules
include "engine/modules/rendering/renderer/renderer_premake.lua"
include "engine/modules/rendering/render_graph/render_graph.lua"
include "engine/modules/rendering/rendering_hardware_interface/rendering_hardware_interface_premake.lua"
include "engine/modules/physics/physics_premake.lua"
include "engine/modules/core/core_premake.lua"
include "engine/modules/audio/audio_premake.lua"
include "engine/modules/input/input_premake.lua"
include "engine/modules/scene/scene_premake.lua"
include "engine/modules/window/window_premake.lua"
include "engine/editor/editor_premake.lua"
