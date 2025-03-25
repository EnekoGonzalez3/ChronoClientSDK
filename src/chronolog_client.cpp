#include "chronolog_client.h"
#include <iostream>
#include <utility>

namespace chronolog {

ChronoLogClient::ChronoLogClient(const std::string& server_address, uint16_t provider_id)
    : m_engine("na+sm", THALLIUM_CLIENT_MODE),
      m_provider_id(provider_id)
{
    // Look up the server endpoint using the given address.
    m_server_ep = m_engine.lookup(server_address);

    // Bind the remote procedures. The names must match those defined on the server.
    m_connect_rpc          = m_engine.define("Connect");
    m_disconnect_rpc       = m_engine.define("Disconnect");
    m_create_chronicle_rpc = m_engine.define("CreateChronicle");
    m_destroy_chronicle_rpc= m_engine.define("DestroyChronicle");
    m_acquire_story_rpc    = m_engine.define("AcquireStory");
    m_release_story_rpc    = m_engine.define("ReleaseStory");
    m_destroy_story_rpc    = m_engine.define("DestroyStory");
    m_get_chronicle_attr_rpc = m_engine.define("GetChronicleAttr");
    m_edit_chronicle_attr_rpc= m_engine.define("EditChronicleAttr");
    m_show_chronicles_rpc  = m_engine.define("ShowChronicles");
    m_show_stories_rpc     = m_engine.define("ShowStories");
}

ChronoLogClient::~ChronoLogClient() {
    m_engine.finalize();
}

ConnectResponseMsg ChronoLogClient::connect(uint32_t client_account, uint32_t client_host_ip, uint32_t client_pid) {
    try {
        // Call the remote "Connect" RPC.
        auto response = m_connect_rpc.on(m_server_ep)(client_account, client_host_ip, client_pid);
        return response.get<ConnectResponseMsg>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::connect] Exception: " << e.what() << std::endl;
        // Return an error response. Adjust the error code and fields as needed.
        return ConnectResponseMsg(-1, ClientId{0});
    }
}

int ChronoLogClient::disconnect(uint32_t client_token) {
    try {
        auto response = m_disconnect_rpc.on(m_server_ep)(client_token);
        return response.get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::disconnect] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::createChronicle(uint32_t client_id, const std::string& chronicle_name,
                                       const std::map<std::string, std::string>& attrs, int flags)
{
    try {
        auto response = m_create_chronicle_rpc.on(m_server_ep)(client_id, chronicle_name, attrs, flags);
        return response.get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::createChronicle] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::destroyChronicle(uint32_t client_id, const std::string& chronicle_name)
{
    try {
        auto response = m_destroy_chronicle_rpc.on(m_server_ep)(client_id, chronicle_name);
        return response.get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::destroyChronicle] Exception: " << e.what() << std::endl;
        return -1;
    }
}

AcquireStoryResponseMsg ChronoLogClient::acquireStory(uint32_t client_id, const std::string& chronicle_name,
                                                      const std::string& story_name, 
                                                      const std::map<std::string, std::string>& attrs,
                                                      int flags)
{
    try {
        auto response = m_acquire_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name, attrs, flags);
        return response.get<AcquireStoryResponseMsg>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::acquireStory] Exception: " << e.what() << std::endl;
        // Return an error response. Adjust according to your response structure.
        return AcquireStoryResponseMsg(-1 /* error code */);
    }
}

int ChronoLogClient::releaseStory(uint32_t client_id, const std::string& chronicle_name,
                                  const std::string& story_name)
{
    try {
        auto response = m_release_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name);
        return response.get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::releaseStory] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::destroyStory(uint32_t client_id, const std::string& chronicle_name,
                                  const std::string& story_name)
{
    try {
        auto response = m_destroy_story_rpc.on(m_server_ep)(client_id, chronicle_name, story_name);
        return response.get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::destroyStory] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::getChronicleAttr(uint32_t client_id, const std::string& chronicle_name,
                                      const std::string& key, std::string &value)
{
    try {
        // Here we assume that the RPC returns a pair: first, the return code; second, the attribute value.
        auto response = m_get_chronicle_attr_rpc.on(m_server_ep)(client_id, chronicle_name, key);
        auto result = response.get<std::pair<int, std::string>>();
        value = result.second;
        return result.first;
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::getChronicleAttr] Exception: " << e.what() << std::endl;
        return -1;
    }
}

int ChronoLogClient::editChronicleAttr(uint32_t client_id, const std::string& chronicle_name,
                                       const std::string& key, const std::string& value)
{
    try {
        auto response = m_edit_chronicle_attr_rpc.on(m_server_ep)(client_id, chronicle_name, key, value);
        return response.get<int>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::editChronicleAttr] Exception: " << e.what() << std::endl;
        return -1;
    }
}

std::vector<std::string> ChronoLogClient::showChronicles(uint32_t client_id)
{
    try {
        auto response = m_show_chronicles_rpc.on(m_server_ep)(client_id);
        return response.get<std::vector<std::string>>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::showChronicles] Exception: " << e.what() << std::endl;
        return std::vector<std::string>(); // return an empty vector on error
    }
}

std::vector<std::string> ChronoLogClient::showStories(uint32_t client_id, const std::string& chronicle_name)
{
    try {
        auto response = m_show_stories_rpc.on(m_server_ep)(client_id, chronicle_name);
        return response.get<std::vector<std::string>>();
    } catch (const std::exception& e) {
        std::cerr << "[ChronoLogClient::showStories] Exception: " << e.what() << std::endl;
        return std::vector<std::string>();
    }
}

} // namespace chronolog