project "Physics"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    UseVendor("Jolt")

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Physics" }