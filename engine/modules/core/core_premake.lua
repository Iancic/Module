project "Core"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    UseVendor("EnTT")

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Core" }

    uses { "Renderer", "Physics" }