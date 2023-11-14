#include <iostream>
#include <fstream>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <iomanip>  // Added for std::setw
#include "./nlohmann/json/json.hpp"

using json = nlohmann::json;

// Function to print version information
void printVersion() {
    std::cout << "###################################################\n";
    std::cout << "#              LunaVPN fu v1.0.0                  #\n";
    std::cout << "###################################################\n\n";
    std::cout << "Welcome to LunaVPN fu - Your Functioning Unit for networking needs!\n\n";
    std::cout << "Copyright © RePass Cloud Pty Ltd 2023\n\n";
    std::cout << "LunaVPN fu v1.0.0\n";
    std::cout << "Using JSON for Modern C++ __VERSION__\n\n";
}

int main(int argc, char *argv[]) {
    // Check for version flag
    if (argc == 2 && std::string(argv[1]) == "-v") {
        printVersion();
        return 0;
    }

    // Default file path
    std::string filePath = "data.json";

    // Check if a filepath is provided as a command-line argument
    if (argc > 1) {
        filePath = argv[1];
    }

    // Get server name
    char hostname[256];
    if (gethostname(hostname, sizeof(hostname)) != 0) {
        std::cerr << "Error getting hostname." << std::endl;
        return 1;
    }

    // Get public IP address
    int sock = socket(AF_INET, SOCK_DGRAM, 0);
    if (sock == -1) {
        std::cerr << "Error creating socket." << std::endl;
        return 1;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(53);  // DNS port
    server.sin_addr.s_addr = inet_addr("8.8.8.8");  // Google's public DNS server

    if (connect(sock, (struct sockaddr *)&server, sizeof(server)) != 0) {
        std::cerr << "Error connecting to socket." << std::endl;
        close(sock);
        return 1;
    }

    struct sockaddr_in localAddress;
    socklen_t addressLength = sizeof(localAddress);
    getsockname(sock, (struct sockaddr *)&localAddress, &addressLength);

    std::string publicIP = inet_ntoa(localAddress.sin_addr);

    close(sock);

    // Create JSON object
    json jsonData;
    jsonData["servername"] = hostname;
    jsonData["publicip"] = publicIP;

    // Write JSON to file
    std::ofstream outFile(filePath);
    if (outFile.is_open()) {
        outFile << std::setw(4) << jsonData << std::endl;
        outFile.close();
        std::cout << filePath << " created successfully." << std::endl;
    } else {
        std::cerr << "Error creating " << filePath << "." << std::endl;
        return 1;
    }

    return 0;
}
