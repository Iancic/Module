project "Window"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"
    files { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    if has_windows then
        UseVendor("SDL3")
    end
    uses { "Core" }
    links { "Core" }

    usage "PUBLIC"
        includedirs { _SCRIPT_DIR .. "/public" }