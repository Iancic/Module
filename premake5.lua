-- Clang tooling
include "tools/clang/clang_premake.lua"

-- Platforms
local has_prospero = os.isfile("platform/prospero/prospero.lua")

-- Workspace
workspace "Module"
    configurations {"debug", "release", "development"}
    platforms {"x64"}
    architecture "x86_64"
    startproject "Module"
    location "build/vs"

    filter "configurations:debug"
        symbols "On"

    filter "configurations:development"
        symbols "On"
        optimize "On"

    filter "configurations:release"
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
