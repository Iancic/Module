project "Physics"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    UseVendor("Jolt")
    uses { "Core" }
    links { "Core" }

    usage "PUBLIC"
        includedirs { _SCRIPT_DIR .. "/public" }