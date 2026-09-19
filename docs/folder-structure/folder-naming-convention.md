# Why private/public instead of include/source

The real advantage is enforcement. With Public/Private, a project that depends on your library can only reach the headers you intended to expose, because the private folder is not on its search path. With include/src, the same separation exists only if everyone follows the convention. Nothing stops code in another project from including an internal header through a relative path or an extra includedirs entry.

That enforcement has three practical effects.

First, you can change internal code freely. If no outside project can include a private header, then renaming, restructuring, or deleting anything in it cannot break other projects. Only the public headers are a commitment.

Second, rebuilds stay contained. When a private header changes, only the library that owns it needs recompiling, because no other project includes it. If internal headers leak into other projects, a change to them forces those projects to recompile too. This point follows from how C++ compilation works rather than from a specific source I looked up.

Third, the interface is readable at a glance. Anyone opening the folder can see what the library offers without reading every header. It also makes distribution simpler: if you ship the library as a compiled binary, you ship the Public folder alongside it and nothing else.

This is inspired by Unreal Engine in it's design.