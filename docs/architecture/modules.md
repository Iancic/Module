An app is built by chaining modules in the engine. 

Modules are the engine building blocks. They are registered using a builder patter on App before the app starts `run`. They receive a `Module Context` instead of having raw acces to the engine. This avoids singletons and global acces of any sorts.

`ModuleContext` exposes:
- `ctx_emplace / ctx_get / ctx_find` — engine context resources
- `view<Cs...>()` — typed ECS views, asserts in debug that components were declared in `access()`
- `create_asset / emplace_asset / get_asset` — asset registry operations
- `raw_registry() / raw_asset_registry()` — although there can be some escape hatches available