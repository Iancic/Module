#pragma once
#include <entt/entt.hpp>
#include <vector>

// Used for scheduling modules in parallel
struct module_access {
	std::vector<entt::id_type> reads;
	std::vector<entt::id_type> writes;
};

class module_context;

// Derive others from this base
class module {
public:
	virtual ~module() = default;

	virtual void init(module_context& reads_writes) = 0;
	virtual void update(module_context& reads_writes, float delta_time) = 0;
	virtual void shutdown(module_context& reads_writes) = 0;

private:
};

// A module's access to assets and the registry 
class module_context {
public:
    explicit module_context(entt::registry& main_registry, entt::registry& asset_registry, const module_access* access)
        : m_registry(main_registry), m_asset_registry(asset_registry), m_access(access) {
    }

    template<typename T, typename... Args>
    T& ctx_emplace(Args&&... args) {
        return m_registry.ctx().emplace<T>(std::forward<Args>(args)...);
    }

    template<typename T>
    T& ctx_get() { return m_registry.ctx().get<T>(); }

    template<typename T>
    T* ctx_find() { return m_registry.ctx().find<T>(); }

    template<typename... Cs>
    auto view() {
        return m_registry.view<Cs...>();
    }

    // A module can create assets
    entt::entity create_asset() { return m_asset_registry.create(); }

    template<typename T, typename... Args>
    T& emplace_asset(entt::entity e, Args&&... args) {
        return m_asset_registry.emplace<T>(e, std::forward<Args>(args)...);
    }

    // A module can retrieve assets
    template<typename T>
    T& get_asset(entt::entity e) { return m_asset_registry.get<T>(e); }

    template<typename T>
    T* try_get_asset(entt::entity e) { return m_asset_registry.try_get<T>(e); }

private:

    entt::registry& m_registry;
    entt::registry& m_asset_registry;
    const module_access* m_access;
};