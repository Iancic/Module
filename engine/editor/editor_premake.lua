project "Editor"
    kind "DynamicLibrary"
    language "C++"
    cppdialect "C++23"
    files { "**.cpp", "**.h" }
    includedirs { "public", "private" }

    usage "PUBLIC"
        includedirs { "%{prj.location}/public" }
        links { "Editor" }