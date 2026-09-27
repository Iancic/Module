project "Rendering Hardware Interface"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }

    usage "PUBLIC"
        includedirs { _SCRIPT_DIR .. "/public" }