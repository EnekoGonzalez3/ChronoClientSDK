#include "chronolog_client.h"
#include <iostream>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>

namespace chronolog {

ChronoLogClient::ChronoLogClient(const std::string& server_address, uint16_t provider_id)
    : m_engine("na+sm", THALLIUM_CLIENT_MODE), m_provider_id(provider_id) {
    m_server_ep = m_engine.lookup(server_address);
    m_connect_rpc = m_engine.define("connect");
    m_disconnect_rpc = m_engine.define("disconnect");
    m_create_chronicle_rpc = m_engine.define("createChronicle");
    m_destroy_chronicle_rpc = m_engine.define("destroyChronicle");
    m_acquire_story_rpc = m_engine.define("acquireStory");
    m_release_story_rpc = m_engine.define("releaseStory");
    m_destroy_story_rpc = m_engine.define("destroyStory");
    m_get_chronicle_attr_rpc = m_engine.define("getChronicleAttr");
    m_edit_chronicle_attr_rpc = m_engine.define("editChronicleAttr");
    m_show_chronicles_rpc = m_engine.define("showChronicles");
    m_show_stories_rpc = m_engine.define("showStories");
}

ChronoLogClient::~ChronoLogClient() {}

ConnectResult ChronoLogClient::connect(uint32_t client_account, uint32_t client_host_ip, uint32_t client_pid) {
    ConnectResult result{ -1, 0 };
    try {
        auto response = m_connect_rpc.on(m_server_ep)(client_account, client_host_ip, client_pid);
        auto res = response.as<std::pair<int, uint32_t>>();
        result.status = res.first;
        result.client_id = res.second;
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::connect] Exception: " << e.what() << std::endl;
    }
    return result;
}

int ChronoLogClient::disconnect(uint32_t client_token) {
    try {
        auto response = m_disconnect_rpc.on(m_server_ep)(client_token);
        return response.as<int>();
    } catch (...) {
        return -1;
    }
}

int ChronoLogClient::createChronicle(uint32_t client_id, const std::string& chronicle_name,
                                     const std::map<std::string, std::string>& attrs, int flags) {
    try {
        return m_create_chronicle_rpc.on(m_server_ep)(client_id, chronicle_name, attrs, flags).as<int>();
    } catch (...) {
        return -1;
    }
}

int ChronoLogClient::destroyChronicle(uint32_t client_id, const std::string& chronicle_name) {
    try {
        return m_destroy_chronicle_rpc.on(m_server_ep)(client_id, chronicle_name).as<int>();
    } catch (...) {
        return -1;
    }
}

AcquireStoryResult ChronoLogClient::acquireStory(uint32_t client_id, const std::string& chronicle_name,
                                                 const std::string& story_name,
                                                 const std::map<std::string, std::string>& attrs, int flags) {
    AcquireStoryResult result{ -1, "" };
    try {
        auto response = m_acquire_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name, attrs, flags);
        auto res = response.as<std::pair<int, std::string>>();
        result.status = res.first;
        result.story_id = res.second;
    } catch (...) {}
    return result;
}

int ChronoLogClient::releaseStory(uint32_t client_id, const std::string& chronicle_name, const std::string& story_name) {
    try {
        return m_release_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name).as<int>();
    } catch (...) {
        return -1;
    }
}

int ChronoLogClient::destroyStory(uint32_t client_id, const std::string& chronicle_name, const std::string& story_name) {
    try {
        return m_destroy_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name).as<int>();
    } catch (...) {
        return -1;
    }
}

int ChronoLogClient::getChronicleAttr(uint32_t client_id, const std::string& chronicle_name,
                                      const std::string& key, std::string& value) {
    try {
        auto result = m_get_chronicle_attr_rpc.on(m_server_ep)(client_id, chronicle_name, key)
                         .as<std::pair<int, std::string>>();
        value = result.second;
        return result.first;
    } catch (...) {
        return -1;
    }
}

int ChronoLogClient::editChronicleAttr(uint32_t client_id, const std::string& chronicle_name,
                                       const std::string& key, const std::string& value) {
    try {
        return m_edit_chronicle_attr_rpc.on(m_server_ep)(client_id, chronicle_name, key, value).as<int>();
    } catch (...) {
        return -1;
    }
}

std::vector<std::string> ChronoLogClient::showChronicles(uint32_t client_id) {
    try {
        return m_show_chronicles_rpc.on(m_server_ep)(client_id).as<std::vector<std::string>>();
    } catch (...) {
        return {};
    }
}

std::vector<std::string> ChronoLogClient::showStories(uint32_t client_id, const std::string& chronicle_name) {
    try {
        return m_show_stories_rpc.on(m_server_ep)(client_id, chronicle_name).as<std::vector<std::string>>();
    } catch (...) {
        return {};
    }
}

} // namespace chronolog