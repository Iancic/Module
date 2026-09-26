project "Core"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    files { "**.cpp", "**.h" }
    includedirs { "public", "private" }

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Core" }

    uses { "Renderer", "Physics" }