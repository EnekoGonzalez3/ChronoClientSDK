#ifndef CHRONOLOG_CLIENT_H
#define CHRONOLOG_CLIENT_H

#include <string>
#include <vector>
#include <map>
#include <utility>
#include <thallium.hpp>

// Include these if you need the definitions for response types.
// You might need to adjust the include paths based on your project setup.
#include "chronolog_types.h"
#include "ConnectResponseMsg.h"
#include "AcquireStoryResponseMsg.h"

namespace chronolog {

class ChronoLogClient {
public:
    // Constructor: initializes the Thallium engine and connects to the remote server.
    ChronoLogClient(const std::string& server_address, uint16_t provider_id);

    // Destructor: finalizes the Thallium engine.
    ~ChronoLogClient();

    // RPC methods corresponding to the server API:

    // Connects a client and returns a ConnectResponseMsg containing a status and ClientId.
    ConnectResponseMsg connect(uint32_t client_account,
                               uint32_t client_host_ip,
                               uint32_t client_pid);

    // Disconnects a client using its token. Returns an integer status code.
    int disconnect(uint32_t client_token);

    // Creates a chronicle with the given name, attributes, and flags.
    // Returns an integer status code.
    int createChronicle(uint32_t client_id,
                        const std::string& chronicle_name,
                        const std::map<std::string, std::string>& attrs,
                        int flags);

    // Destroys a chronicle identified by its name. Returns an integer status code.
    int destroyChronicle(uint32_t client_id,
                         const std::string& chronicle_name);

    // Acquires a story in a chronicle with the given attributes and flags.
    // Returns an AcquireStoryResponseMsg.
    AcquireStoryResponseMsg acquireStory(uint32_t client_id,
                                         const std::string& chronicle_name,
                                         const std::string& story_name,
                                         const std::map<std::string, std::string>& attrs,
                                         int flags);

    // Releases a previously acquired story. Returns an integer status code.
    int releaseStory(uint32_t client_id,
                     const std::string& chronicle_name,
                     const std::string& story_name);

    // Destroys a story in the specified chronicle. Returns an integer status code.
    int destroyStory(uint32_t client_id,
                     const std::string& chronicle_name,
                     const std::string& story_name);

    // Retrieves an attribute value for a chronicle.
    // The attribute value is returned via the output parameter 'value' along with a status code.
    int getChronicleAttr(uint32_t client_id,
                         const std::string& chronicle_name,
                         const std::string& key,
                         std::string &value);

    // Edits or sets an attribute for a chronicle. Returns an integer status code.
    int editChronicleAttr(uint32_t client_id,
                          const std::string& chronicle_name,
                          const std::string& key,
                          const std::string& value);

    // Returns a list of chronicle names for the client.
    std::vector<std::string> showChronicles(uint32_t client_id);

    // Returns a list of story names in a given chronicle.
    std::vector<std::string> showStories(uint32_t client_id,
                                         const std::string& chronicle_name);

private:
    // Thallium engine instance used for communication.
    thallium::engine m_engine;
    // The remote endpoint for the ChronoLog server.
    thallium::endpoint m_server_ep;
    // The provider id for the service.
    uint16_t m_provider_id;

    // Remote procedure handles.
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

#endif // CHRONOLOG_CLIENT_HPP