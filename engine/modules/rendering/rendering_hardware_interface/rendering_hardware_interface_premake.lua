project "Rendering Hardware Interface"
    kind "SharedLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Rendering Hardware Interface" }