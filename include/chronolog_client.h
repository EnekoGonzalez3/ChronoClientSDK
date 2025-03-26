#ifndef CHRONOLOG_CLIENT_H
#define CHRONOLOG_CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <thallium.hpp>

namespace chronolog {

struct ConnectResult {
    int status;
    uint32_t client_id;
};

struct AcquireStoryResult {
    int status;
    std::string story_id;
};

class ChronoLogClient {
public:
    ChronoLogClient(const std::string& server_address, uint16_t provider_id);
    ~ChronoLogClient();

    ConnectResult connect(uint32_t client_account,
                          uint32_t client_host_ip,
                          uint32_t client_pid);

    int disconnect(uint32_t client_token);

    int createChronicle(uint32_t client_id,
                        const std::string& chronicle_name,
                        const std::map<std::string, std::string>& attrs,
                        int flags);

    int destroyChronicle(uint32_t client_id,
                         const std::string& chronicle_name);

    AcquireStoryResult acquireStory(uint32_t client_id,
                                    const std::string& chronicle_name,
                                    const std::string& story_name,
                                    const std::map<std::string, std::string>& attrs,
                                    int flags);

    int releaseStory(uint32_t client_id,
                     const std::string& chronicle_name,
                     const std::string& story_name);

    int destroyStory(uint32_t client_id,
                     const std::string& chronicle_name,
                     const std::string& story_name);

    int getChronicleAttr(uint32_t client_id,
                         const std::string& chronicle_name,
                         const std::string& key,
                         std::string& value);

    int editChronicleAttr(uint32_t client_id,
                          const std::string& chronicle_name,
                          const std::string& key,
                          const std::string& value);

    std::vector<std::string> showChronicles(uint32_t client_id);
    std::vector<std::string> showStories(uint32_t client_id,
                                         const std::string& chronicle_name);

private:
    thallium::engine m_engine;
    thallium::endpoint m_server_ep;
    uint16_t m_provider_id;

    thallium::remote_procedure m_connect_rpc;
    thallium::remote_procedure m_disconnect_rpc;
    thallium::remote_procedure m_create_chronicle_rpc;
    thallium::remote_procedure m_destroy_chronicle_rpc;
    thallium::remote_procedure m_acquire_story_rpc;
    thallium::remote_procedure m_release_story_rpc;
    thallium::remote_procedure m_destroy_story_rpc;
    thallium::remote_procedure m_get_chronicle_attr_rpc;
    thallium::remote_procedure m_edit_chronicle_attr_rpc;
    thallium::remote_procedure m_show_chronicles_rpc;
    thallium::remote_procedure m_show_stories_rpc;
};

} // namespace chronolog

#endif // CHRONOLOG_CLIENT_H