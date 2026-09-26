require "ecc/ecc"

-- Only the engine code is linted/formatted.
local MODULE_ROOTS = { "engine/modules", "engine/editor" }
local CLANG_FORMAT_CONFIG = path.join(_MAIN_SCRIPT_DIR, "tools/clang/.clang-format")
local CLANG_TIDY_CONFIG   = path.join(_MAIN_SCRIPT_DIR, "tools/clang/.clang-tidy")

local function collectFiles()
    local files = {}
    for _, root in ipairs(MODULE_ROOTS) do
        files = table.join(files, os.matchfiles(root .. "/**.cpp"))
        files = table.join(files, os.matchfiles(root .. "/**.hpp"))
    end
    return files
end

--- CLANG: Tidy, Format
--- Run: premake5 format
--- Like running: clang-format -i -style=file:tools/clang/.clang-format engine/modules/audio/private/audio.cpp
--- But for every source and hpp file under engine/modules and engine/editor
newaction{
    trigger = "format",
    description = "Run clang-format on all engine module/editor source files (excludes vendor).",
    execute = function ()
        local files = collectFiles()
        for _, f in ipairs(files) do
            os.execute(string.format('clang-format -i -style=file:%s "%s"', CLANG_FORMAT_CONFIG, f))
        end
    end
}

--- Run: premake5 tidy
--- Like running: clang-tidy -p premake --config-file=tools/clang/.clang-tidy engine/modules/audio/private/audio.cpp
--- But for every source and hpp file under engine/modules and engine/editor
newaction{
    trigger = "tidy",
    description = "Run clang-tidy on all engine module/editor source files (excludes vendor).",
    execute = function()
        local files = collectFiles()
        for _, f in ipairs(files) do
            os.execute(string.format('clang-tidy -p "%s" --config-file=%s "%s"', _MAIN_SCRIPT_DIR, CLANG_TIDY_CONFIG, f))
        end
    end
}
