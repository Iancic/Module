# Vendor Folder

The shared dependencies like header only libraries or engine libraries like entt are under the root vendor folder.

For platform specific libraries like SDL, whic is not used on consoles, they are found under their specific `platform/pc/vendor`.

This allows me to very easily include or exclude vendors that might be under NDA or even exclude them from compiling.

Also makes it easy to see which dependencies are portable. The rule is simple.