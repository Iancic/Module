project "Audio"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    UseVendor("FMOD")

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Audio" }