#include "pch.hpp"
#include <modules/audio_module.hpp>
#include <core/audio_components.hpp>
#include <core/component_serializer.hpp>
#include <core/transform.hpp>
#include <core/camera.hpp>
#include <core/logger.hpp>

#include <fmod/fmod_studio.hpp>
#include <fmod/fmod.hpp>
#include <fmod/fmod_errors.h>

#include <tinygltf/json.hpp>

#include <unordered_map>
#include <vector>
#include <string>
#include <random>

namespace {

bool fmod_ok(FMOD_RESULT r, const char* ctx) {
    if (r == FMOD_OK) return true;
    Logger::Error("[Audio] FMOD error in ", ctx, ": ", FMOD_ErrorString(r));
    return false;
}

FMOD_VECTOR to_fmod(const glm::vec3& v) { return { v.x, v.y, v.z }; }

int random_id() {
    static std::mt19937 rng(std::random_device{}());
    return static_cast<int>(std::uniform_int_distribution<uint32_t>{}(rng));
}

}

struct AudioModule::Impl {
    FMOD::Studio::System* studio      = nullptr;
    FMOD::System*         core        = nullptr;
    FMOD::SoundGroup*     group_sfx   = nullptr;
    FMOD::SoundGroup*     group_music = nullptr;
    FMOD::Studio::Bus*    master_bus  = nullptr;

    std::unordered_map<int, FMOD::Studio::Bank*>          banks;
    std::unordered_map<int, FMOD::Studio::EventInstance*> events;
    std::unordered_map<int, FMOD::Sound*>                 sounds;

    std::vector<std::string>                                  bank_names;
    std::unordered_map<std::string, std::vector<std::string>> bank_to_events;
    std::unordered_map<std::string, std::vector<std::string>> event_to_params;

    std::vector<int> events_to_remove;
    int next_event_id = 0;
};

AudioModule::AudioModule() : m_impl(new Impl{}) {}
AudioModule::~AudioModule() { delete m_impl; }

ModuleAccess AudioModule::access() const
{
    ModuleAccess a;
    a.reads  = module_access::ids<Transform, ActiveCamera>();
    a.writes = module_access::ids<AudioSource, AudioSourceRuntime>();
    return a;
}

uint32_t AudioModule::studio_init_flags() const { return FMOD_STUDIO_INIT_NORMAL; }

void AudioModule::init(ModuleContext& /*ctx*/)
{
    Logger::Log("[Audio] init");
    platform_preinit();

    FMOD_RESULT r = FMOD::Studio::System::create(&m_impl->studio);
    if (!fmod_ok(r, "Studio::System::create")) return;

    r = m_impl->studio->initialize(512,
        static_cast<FMOD_STUDIO_INITFLAGS>(studio_init_flags()),
        FMOD_INIT_NORMAL, nullptr);
    if (!fmod_ok(r, "Studio::System::initialize")) return;

    fmod_ok(m_impl->studio->getCoreSystem(&m_impl->core), "getCoreSystem");
    m_impl->core->createSoundGroup("SFX",   &m_impl->group_sfx);
    m_impl->core->createSoundGroup("Music", &m_impl->group_music);
    m_impl->studio->getBus("bus:/", &m_impl->master_bus);

    ComponentSerializationRegistry::instance().register_type(
        "AudioSource",
        [](entt::registry& reg, entt::entity e) -> nlohmann::json {
            const auto* src = reg.try_get<AudioSource>(e);
            if (!src) return nullptr;
            nlohmann::json j = nlohmann::json::array();
            for (const auto& ev : src->events) {
                nlohmann::json ej;
                ej["id"]            = ev.id;
                ej["bank"]          = ev.bank_name;
                ej["event"]         = ev.event_name;
                ej["spatial"]       = ev.play_spatial;
                ej["play_on_start"] = ev.play_on_start;
                ej["looping"]       = ev.looping;
                nlohmann::json params = nlohmann::json::array();
                for (const auto& p : ev.parameters) {
                    nlohmann::json pj;
                    pj["name"]  = p.name;
                    pj["value"] = p.value;
                    params.push_back(std::move(pj));
                }
                ej["parameters"] = std::move(params);
                j.push_back(std::move(ej));
            }
            return j;
        },
        [](entt::registry& reg, entt::entity e, const nlohmann::json& j) {
            auto& src = reg.get_or_emplace<AudioSource>(e);
            src.events.clear();
            for (const auto& ej : j) {
                AudioEvent ev;
                ev.id            = ej.value("id",            random_id());
                ev.bank_name     = ej.value("bank",          std::string{});
                ev.event_name    = ej.value("event",         std::string{});
                ev.play_spatial  = ej.value("spatial",       false);
                ev.play_on_start = ej.value("play_on_start", false);
                ev.looping       = ej.value("looping",       false);
                if (ej.contains("parameters")) {
                    for (const auto& pj : ej["parameters"]) {
                        AudioParameter p;
                        p.name  = pj.value("name",  std::string{});
                        p.value = pj.value("value", 0.0f);
                        ev.parameters.push_back(p);
                    }
                }
                src.events.push_back(std::move(ev));
            }
        },
        [](entt::registry& reg, entt::entity e) { return reg.all_of<AudioSource>(e); }
    );

    Logger::Log("[Audio] init complete");
}

void AudioModule::update(ModuleContext& ctx, float /*dt*/)
{
    if (!m_impl->studio) return;
    m_impl->studio->update();
    tick_spatial_sources(ctx.raw_registry());
    remove_stopped_events();
}

void AudioModule::shutdown(ModuleContext& /*ctx*/)
{
    if (!m_impl->studio) return;

    for (auto& [id, ev] : m_impl->events)
        if (ev) { ev->stop(FMOD_STUDIO_STOP_IMMEDIATE); ev->release(); }
    m_impl->events.clear();

    for (auto& [id, bank] : m_impl->banks)
        if (bank) bank->unload();
    m_impl->banks.clear();

    if (m_impl->group_sfx)   m_impl->group_sfx->release();
    if (m_impl->group_music) m_impl->group_music->release();

    m_impl->studio->release();
    m_impl->studio = nullptr;
    Logger::Log("[Audio] shutdown");
}

void AudioModule::load_bank(const std::string& path)
{
    const int hash = static_cast<int>(std::hash<std::string>{}(path));
    if (m_impl->banks.count(hash)) return;

    FMOD::Studio::Bank* bank = nullptr;
    FMOD_RESULT r = m_impl->studio->loadBankFile(path.c_str(), FMOD_STUDIO_LOAD_BANK_NORMAL, &bank);

    if (r == FMOD_ERR_VERSION) {
        unsigned int ver;
        m_impl->core->getVersion(&ver);
        Logger::Error("[Audio] bank version mismatch for ", path,
                      " — FMOD runtime ", (ver >> 16), ".", ((ver >> 8) & 0xff), ".", (ver & 0xff));
        return;
    }
    if (!fmod_ok(r, "loadBankFile")) return;

    bank->loadSampleData();
    m_impl->studio->flushSampleLoading();
    m_impl->banks[hash] = bank;

    int count = 0;
    bank->getEventCount(&count);
    if (count > 0) {
        std::vector<FMOD::Studio::EventDescription*> descs(static_cast<size_t>(count));
        bank->getEventList(descs.data(), count, &count);
        const char* prefix = "event:/";
        const size_t plen  = strlen(prefix);

        for (auto* desc : descs) {
            char buf[256]{};
            int ret = 0;
            desc->getPath(buf, sizeof(buf), &ret);
            std::string event_path = (strncmp(buf, prefix, plen) == 0)
                                     ? std::string(buf + plen) : std::string(buf);
            m_impl->bank_to_events[path].push_back(event_path);

            int pcount = 0;
            desc->getParameterDescriptionCount(&pcount);
            for (int p = 0; p < pcount; ++p) {
                FMOD_STUDIO_PARAMETER_DESCRIPTION pd{};
                desc->getParameterDescriptionByIndex(p, &pd);
                m_impl->event_to_params[event_path].push_back(pd.name);
            }
        }
    }

    m_impl->bank_names.push_back(path);
    Logger::Log("[Audio] loaded bank: ", path);
}

void AudioModule::unload_bank(const std::string& path)
{
    const int hash = static_cast<int>(std::hash<std::string>{}(path));
    auto it = m_impl->banks.find(hash);
    if (it == m_impl->banks.end()) {
        Logger::Warning("[Audio] unload_bank: not found: ", path);
        return;
    }
    it->second->unload();
    m_impl->banks.erase(it);
}

int AudioModule::create_event(const std::string& event_name)
{
    FMOD::Studio::EventDescription* desc = nullptr;
    if (!fmod_ok(m_impl->studio->getEvent(("event:/" + event_name).c_str(), &desc), "getEvent"))
        return -1;

    FMOD::Studio::EventInstance* inst = nullptr;
    if (!fmod_ok(desc->createInstance(&inst), "createInstance"))
        return -1;

    const int id = m_impl->next_event_id++;
    m_impl->events[id] = inst;
    return id;
}

void AudioModule::start_event(int id)
{
    auto it = m_impl->events.find(id);
    if (it != m_impl->events.end()) it->second->start();
}

void AudioModule::stop_event(int id, bool allow_fadeout)
{
    auto it = m_impl->events.find(id);
    if (it != m_impl->events.end())
        it->second->stop(allow_fadeout ? FMOD_STUDIO_STOP_ALLOWFADEOUT : FMOD_STUDIO_STOP_IMMEDIATE);
}

void AudioModule::release_event(int id)
{
    auto it = m_impl->events.find(id);
    if (it == m_impl->events.end()) return;
    it->second->release();
    m_impl->events.erase(it);
}

void AudioModule::set_parameter(const std::string& param, float value, int event_id)
{
    if (event_id < 0) {
        m_impl->studio->setParameterByName(param.c_str(), value);
        return;
    }
    auto it = m_impl->events.find(event_id);
    if (it != m_impl->events.end())
        it->second->setParameterByName(param.c_str(), value);
}

bool AudioModule::is_valid(int id) const
{
    auto it = m_impl->events.find(id);
    if (it == m_impl->events.end()) return false;
    return it->second->isValid();
}

void AudioModule::start_audio_sources(entt::registry& registry)
{
    auto view = registry.view<AudioSource, Transform>();
    for (auto [e, src, transform] : view.each()) {
        auto& rt = registry.get_or_emplace<AudioSourceRuntime>(e);
        rt.events.clear();

        for (auto& ev : src.events) {
            if (ev.event_name.empty()) continue;

            AudioEventRuntime art{};
            art.fmod_id = create_event(ev.event_name);
            if (art.fmod_id < 0) continue;

            if (ev.play_on_start) {
                for (const auto& p : ev.parameters)
                    set_parameter(p.name, p.value, art.fmod_id);

                if (ev.play_spatial) {
                    FMOD_3D_ATTRIBUTES attr{};
                    attr.position = to_fmod(transform.translation());
                    attr.forward  = to_fmod(transform.forward());
                    attr.up       = to_fmod(transform.up());
                    m_impl->events[art.fmod_id]->set3DAttributes(&attr);
                }

                start_event(art.fmod_id);
                art.state = AudioEventState::Playing;

                if (!ev.looping) {
                    release_event(art.fmod_id);
                    art.state = AudioEventState::Released;
                }
            }

            rt.events[ev.id] = art;
        }
    }
}

void AudioModule::stop_audio_sources(entt::registry& registry)
{
    auto view = registry.view<AudioSource, AudioSourceRuntime>();
    for (auto [e, src, rt] : view.each()) {
        for (auto& ev : src.events) {
            auto it = rt.events.find(ev.id);
            if (it == rt.events.end()) continue;
            if (it->second.state == AudioEventState::Playing) {
                stop_event(it->second.fmod_id);
                release_event(it->second.fmod_id);
            }
        }
        rt.events.clear();
    }
}

void AudioModule::tick_spatial_sources(entt::registry& registry)
{
    auto cam_view = registry.view<ActiveCamera, Transform>();
    for (auto [e, transform] : cam_view.each()) {
        FMOD_3D_ATTRIBUTES attr{};
        attr.position = to_fmod(transform.translation());
        attr.forward  = to_fmod(transform.forward());
        attr.up       = to_fmod(transform.up());
        m_impl->studio->setListenerAttributes(0, &attr);
        break;
    }

    auto src_view = registry.view<AudioSource, AudioSourceRuntime, Transform>();
    for (auto [e, src, rt, transform] : src_view.each()) {
        for (const auto& ev : src.events) {
            auto it = rt.events.find(ev.id);
            if (it == rt.events.end() || it->second.state != AudioEventState::Playing) continue;

            if (ev.play_spatial) {
                FMOD_3D_ATTRIBUTES attr{};
                attr.position = to_fmod(transform.translation());
                attr.forward  = to_fmod(transform.forward());
                attr.up       = to_fmod(transform.up());
                auto ev_it = m_impl->events.find(it->second.fmod_id);
                if (ev_it != m_impl->events.end())
                    ev_it->second->set3DAttributes(&attr);
            }
            for (const auto& p : ev.parameters)
                set_parameter(p.name, p.value, it->second.fmod_id);
        }
    }
}

void AudioModule::remove_stopped_events()
{
    for (size_t i = 0; i < m_impl->events_to_remove.size(); ) {
        int id = m_impl->events_to_remove[i];
        auto it = m_impl->events.find(id);
        if (it != m_impl->events.end()) {
            it->second->release();
            m_impl->events.erase(it);
        }
        m_impl->events_to_remove[i] = m_impl->events_to_remove.back();
        m_impl->events_to_remove.pop_back();
    }
}
