project "Window"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    files { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    if has_windows then
        UseVendor("SDL3")
    end

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Window" }