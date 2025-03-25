#include "chronolog_client.h"
#include <iostream>

using namespace chronolog;

int main() {
    // Assume the server is reachable at the given address.
    std::string server_address = "na+sm://server_endpoint";  // Adjust as needed
    uint16_t provider_id = 0; // Set the provider id based on your deployment

    ChronoLogClient client(server_address, provider_id);

    // Example: Connect a client
    uint32_t client_account = 1234;
    uint32_t client_host_ip = 0x7F000001; // 127.0.0.1 in hex
    uint32_t client_pid = 4321;
    int connect_rc = client.connect(client_account, client_host_ip, client_pid);

    if (connect_rc == 0) {
        std::cout << "Connected successfully!" << std::endl;
    } else {
        std::cout << "Connection failed with code: " << connect_rc << std::endl;
    }

    // Example: Create a chronicle
    int flags = 0;
    std::map<std::string, std::string> attrs = { {"description", "Test chronicle"} };
    int create_rc = client.createChronicle(client_account, "TestChronicle", attrs, flags);
    if (create_rc == 0) {
        std::cout << "Chronicle created!" << std::endl;
    } else {
        std::cout << "Failed to create chronicle: " << create_rc << std::endl;
    }

    // Example: Disconnect
    int disconnect_rc = client.disconnect(client_account);
    if (disconnect_rc == 0) {
        std::cout << "Disconnected successfully!" << std::endl;
    } else {
        std::cout << "Disconnection failed: " << disconnect_rc << std::endl;
    }

    return 0;
}