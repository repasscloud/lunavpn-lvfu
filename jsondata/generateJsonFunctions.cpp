#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring> // for std::strerror
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <filesystem> // Added for filesystem operations
#include "nlohmann/json.hpp"

using json = nlohmann::json;
namespace fs = std::filesystem;

// Function to generate json
void generateJson(const std::string& filePath) {
    // Check if the file already exists
    if (fs::exists(filePath)) {
        // Remove the existing file
        if (!fs::remove(filePath)) {
            std::cerr << "Error deleting existing file: " << std::strerror(errno) << std::endl;
            exit(1);
        }
    }

    // Get server name
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "Error getting hostname." << std::endl;
        exit(1);
    }

    // Get public IP address
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "Error creating socket." << std::endl;
        exit(1);
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(53);  // DNS port
    server.sin_addr.s_addr = inet_addr("8.8.8.8");  // Google's public DNS server

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
        std::cerr << "Error connecting to socket." << std::endl;
        close(sock);
        exit(1);
    }

    struct sockaddr_in localAddress;
    socklen_t addressLength = sizeof(localAddress);
    getsockname(sock, (struct sockaddr *)&localAddress, &addressLength);

    std::string publicIP = inet_ntoa(localAddress.sin_addr);

    close(sock);

    // Read server.uuid file
    std::ifstream uuidFile("/app/html/server.uuid");
    std::string serverUid;
    if (uuidFile.is_open()) {
        getline(uuidFile, serverUid);
        uuidFile.close();
    }

    // Read server_type file
    std::ifstream serverTypeFile("/app/scripts/server_type");
    std::string serverType;
    if (serverTypeFile.is_open()) {
        getline(serverTypeFile, serverType);
        serverTypeFile.close();
    }

    // Create JSON object
    json jsonData;
    jsonData["servername"] = hostname;
    jsonData["publicip"] = publicIP;
    jsonData["serveruid"] = serverUid;
    jsonData["servertype"] = serverType;

    // Write JSON to file
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << std::setw(4) << jsonData << std::endl;
        outFile.close();
        std::cout << filePath << " created successfully." << std::endl;
    } else {
        std::cerr << "Error creating " << filePath << "." << std::endl;
        exit(1);
    }
}
