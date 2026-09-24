#pragma once
#include <entt/entt.hpp>
#include <algorithm>
#include <cassert>
#include <typeindex>
#include <type_traits>
#include <vector>

class module_context;

struct module_access {
    std::vector<entt::id_type> reads;
    std::vector<entt::id_type> writes;
};

class module {
public:
    virtual ~module() = default;

    virtual void init(module_context& ctx)             = 0;
    virtual void update(module_context& ctx, float dt) = 0;
    virtual void shutdown(module_context& ctx)         = 0;

    virtual std::vector<std::type_index> requires_modules() const { return {}; }
    virtual module_access access() const { return {}; }
};

class module_context {
public:
    explicit module_context(entt::registry& reg, entt::registry& asset_reg, const ModuleAccess* access)
        : m_registry(reg), m_asset_registry(asset_reg), m_access(access) {}

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
        assert_declared<Cs...>();
        return m_registry.view<Cs...>();
    }

    entt::entity create_asset() { return m_asset_registry.create(); }

    template<typename T, typename... Args>
    T& emplace_asset(entt::entity e, Args&&... args) {
        return m_asset_registry.emplace<T>(e, std::forward<Args>(args)...);
    }

    template<typename T>
    T& get_asset(entt::entity e) { return m_asset_registry.get<T>(e); }

    template<typename T>
    T* try_get_asset(entt::entity e) { return m_asset_registry.try_get<T>(e); }

    entt::registry& raw_registry()       { return m_registry; }
    entt::registry& raw_asset_registry() { return m_asset_registry; }

private:
    template<typename... Cs>
    void assert_declared() {
#ifndef NDEBUG
        if (!m_access) return;
        (assert_one<Cs>(), ...);
#endif
    }

    template<typename C>
    void assert_one() {
#ifndef NDEBUG
        using bare = std::remove_const_t<C>;
        const auto id = entt::type_hash<bare>::value();
        const auto& r = m_access->reads;
        const auto& w = m_access->writes;
        const bool ok =
            std::find(r.begin(), r.end(), id) != r.end() ||
            std::find(w.begin(), w.end(), id) != w.end();
        assert(ok && "ModuleContext::view<C>() used for an undeclared component");
#endif
    }

    entt::registry&     m_registry;
    entt::registry&     m_asset_registry;
    const module_access* m_access;
};
