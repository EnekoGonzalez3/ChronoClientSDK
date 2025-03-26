#include "chronolog_client.h"
#include <iostream>
#include <cereal/types/string.hpp>
#include <cereal/types/vector.hpp>
#include <cereal/types/map.hpp>
#include <cereal/types/utility.hpp>

namespace chronolog {

ConnectResponseMsg ChronoLogClient::connect(uint32_t client_account, uint32_t client_host_ip, uint32_t client_pid) {
    try {
        auto response = m_connect_rpc.on(m_server_ep)(client_account, client_host_ip, client_pid);
        return response.as<ConnectResponseMsg>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::connect] Exception: " << e.what() << std::endl;
        return ConnectResponseMsg();
    }
}

int ChronoLogClient::disconnect(uint32_t client_token) {
    try {
        auto response = m_disconnect_rpc.on(m_server_ep)(client_token);
        return response.as<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::disconnect] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::createChronicle(uint32_t client_id, const std::string& chronicle_name,
                                     const std::map<std::string, std::string>& attrs, int flags) {
    try {
        auto response = m_create_chronicle_rpc.on(m_server_ep)(client_id, chronicle_name, attrs, flags);
        return response.as<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::createChronicle] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::destroyChronicle(uint32_t client_id, const std::string& chronicle_name) {
    try {
        auto response = m_destroy_chronicle_rpc.on(m_server_ep)(client_id, chronicle_name);
        return response.as<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::destroyChronicle] Exception: " << e.what() << std::endl;
        return -1;
    }
}

AcquireStoryResponseMsg ChronoLogClient::acquireStory(uint32_t client_id, const std::string& chronicle_name,
                                                      const std::string& story_name,
                                                      const std::map<std::string, std::string>& attrs, int flags) {
    try {
        auto response = m_acquire_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name, attrs, flags);
        return response.as<AcquireStoryResponseMsg>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::acquireStory] Exception: " << e.what() << std::endl;
        return AcquireStoryResponseMsg();
    }
}

int ChronoLogClient::releaseStory(uint32_t client_id, const std::string& chronicle_name, const std::string& story_name) {
    try {
        auto response = m_release_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name);
        return response.as<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::releaseStory] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::destroyStory(uint32_t client_id, const std::string& chronicle_name, const std::string& story_name) {
    try {
        auto response = m_destroy_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name);
        return response.as<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::destroyStory] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::getChronicleAttr(uint32_t client_id, const std::string& chronicle_name,
                                      const std::string& key, std::string& value) {
    try {
        auto response = m_get_chronicle_attr_rpc.on(m_server_ep)(client_id, chronicle_name, key);
        auto result = response.as<std::pair<int, std::string>>();
        value = result.second;
        return result.first;
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::getChronicleAttr] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::editChronicleAttr(uint32_t client_id, const std::string& chronicle_name,
                                       const std::string& key, const std::string& value) {
    try {
        auto response = m_edit_chronicle_attr_rpc.on(m_server_ep)(client_id, chronicle_name, key, value);
        return response.as<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::editChronicleAttr] Exception: " << e.what() << std::endl;
        return -1;
    }
}

std::vector<std::string> ChronoLogClient::showChronicles(uint32_t client_id) {
    try {
        auto response = m_show_chronicles_rpc.on(m_server_ep)(client_id);
        return response.as<std::vector<std::string>>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::showChronicles] Exception: " << e.what() << std::endl;
        return {};
    }
}

std::vector<std::string> ChronoLogClient::showStories(uint32_t client_id, const std::string& chronicle_name) {
    try {
        auto response = m_show_stories_rpc.on(m_server_ep)(client_id, chronicle_name);
        return response.as<std::vector<std::string>>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::showStories] Exception: " << e.what() << std::endl;
        return {};
    }
}

} // namespace chronolog
