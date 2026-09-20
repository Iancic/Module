require "ecc/ecc"

--- CLANG: Tidy, Format

--- Run: premake5 format
--- Like running: clang-format -p . src/Renderer.cpp
--- But for every source and hpp file
newaction{
    trigger = "format",
    description = "Run clang format on all source files.",
    execute = function ()
        local files = table.join(os.matchfiles("src/**.cpp"), os.matchfiles("src/**.hpp"))
        for _, f in ipairs(files) do
            os.execute("clang-format -i" .. f)
        end
    end
}

--- Run: premake5 tidy
--- Like running: clang-tidy -p . src/Renderer.cpp
--- But for every source and hpp file
newaction{
    trigger = "tidy",
    description = "Run clang tidy on all source files.",
    execute = function()
        local files = table.join(os.matchfiles("src/**.cpp"), os.matchfiles("src/**.hpp"))
        for _, f in ipairs(files) do
            os.execute("clang-tidy -p" .. f)
        end
    end
}