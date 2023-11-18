#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring> // for std::strerror
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <filesystem> // Added for filesystem operations
#include <sstream>
#include "nlohmann/json.hpp"
#include <vector>
#include <mtrp/base64.hpp>

using json = nlohmann::json;
namespace fs = std::filesystem;
namespace b64 = mtrp;

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
    std::ifstream uuidFile("/app/lunavpn/server.uuid");
    std::string serverUid;
    if (uuidFile.is_open()) {
        getline(uuidFile, serverUid);
        uuidFile.close();
    }

    // Read server_type file
    std::ifstream serverTypeFile("/app/lunavpn/server.type");
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

    // squid.creds
    const std::string squidCredsFilePath = "/app/squid/squid.creds";
    if (fs::exists(squidCredsFilePath)) {
        // read the contents then close fileStream
        std::ifstream fileStream(squidCredsFilePath);
        std::string fileContent;
        std::getline(fileStream, fileContent);
        fileStream.close();
        // extract username and password
        std::istringstream iss(fileContent);
        std::string username, password;
        std::getline(iss, username, ';');
        std::getline(iss, password);

        // Create the nested "proxy_login" object
        nlohmann::json proxyLogin;
        proxyLogin["username"] = username;
        proxyLogin["password"] = password;

        // Add the "proxy_login" object to the main JSON object
        jsonData["proxy_login"] = proxyLogin;
    }

    // wireguard Peers
    const std::string wgPeers = "/app/wg/config";
    if (fs::is_directory(wgPeers)) {
        // Create the nested "wireguard" object
        nlohmann::json wireguard;

        // create the wireguard peerNames array
        std::vector<std::string> peerNames;

        // Iterate over the contents of the directory
        for (const auto& entry : std::filesystem::directory_iterator(wgPeers)) {
            // Check if the entry is a directory and matches the naming convention
            if (entry.is_directory()) {
                std::string folderName = entry.path().filename().string();

                // Check if the folder name matches the convention "peerX"
                if (folderName.substr(0, 4) == "peer" && folderName.size() > 4) {
                    std::string remainingDigits = folderName.substr(4);

                    // Check if the remaining part is a valid integer
                    if (std::all_of(remainingDigits.begin(), remainingDigits.end(), ::isdigit)) {
                        peerNames.push_back(folderName);
                    }
                }
            }

            // Output the collected peer names
            if (!peerNames.empty()) {
                for (const auto& peerName : peerNames) {
                    std::string peerPNGName = peerName + "PNG";
                    std::string peerConfName = peerName + "Conf";

                    std::string peerPngFile = wgPeers + "/" + peerName + ".png";
                    std::string peerConfFile = wgPeers + "/" + peerName + ".conf";

                    std::ifstream file(peerPngFile, std::ios::binary);
                    if (!file.is_open()) {
                        std::cerr << "Error opening file: " << peerPngFile << std::endl;
                        exit(1);
                    }

                    // read the file content into a vector
                    std::vector<unsigned char> fileContent(
                        (std::istreambuf_iterator<char>(file)),
                        std::istreambuf_iterator<char>()
                    );

                    // Close the file
                    file.close();

                    // Convert the vector to a base64 string
                    //std::string base64String = base64_encode(fileContent.data(), fileContent.size());
                    std::string base64String;
                    b64::encode(fileContent.data(), fileContent.size(), base64String);

                    // Open the file
                    std::ifstream file2(peerConfFile);
                    if (!file2.is_open()) {
                        std::cerr << "Error opening file: " << peerConfFile << std::endl;
                        exit(1);
                    }

                    // Read the file content into a string
                    std::stringstream buffer;
                    buffer << file2.rdbuf();
                    std::string peerConfContents = buffer.str();

                    // Close the file
                    file2.close();

                    wireguard[peerPNGName] = base64String;
                    wireguard[peerConfName] = peerConfContents;
                }
            }
        }

        // Add the "proxy_login" object to the main JSON object
        jsonData["wg_peers"] = wireguard;
    }

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
