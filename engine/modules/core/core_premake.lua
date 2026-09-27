project "Core"
    kind "StaticLib"
    language "C++"
    cppdialect "C++23"
    files  { "**.cpp", "**.h", "**.hpp" }
    includedirs { "public", "private" }
    UseVendor("EnTT")

    usage "PUBLIC"
        includedirs { _SCRIPT_DIR .. "/public" }
        UseVendor("EnTT")