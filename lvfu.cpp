#include <arpa/inet.h>
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "./nlohmann/json/json.hpp"

using json = nlohmann::json;

// Function to print version information
void printVersion() {
    std::cout << "###################################################\n";
    std::cout << "#              LunaVPN fu v1.1.0                  #\n";
    std::cout << "###################################################\n\n";
    std::cout << "Welcome to LunaVPN fu - Your Functioning Unit for networking needs!\n";
    std::cout << "Copyright © RePass Cloud Pty Ltd 2023\n\n";
    std::cout << "LunaVPN fu v1.1.0\n";
    std::cout << "Using JSON for Modern C++ ___VERSION___\n\n";
}

// Function to print help information
void printHelp() {
        std::cout << "###################################################\n";
    std::cout << "#              LunaVPN fu v1.1.0                  #\n";
    std::cout << "###################################################\n\n";
    std::cout << "Welcome to LunaVPN fu - Your Functioning Unit for networking needs!\n";
    std::cout << "Copyright © RePass Cloud Pty Ltd 2023\n\n";
    std::cout << "Usage: LunaVPN_fu [-v] | [-gen] | [-u <URL> -h <HEADER>] | [<path_to_output.json>]\n\n";
    std::cout << "-v                      Display version information\n";
    std::cout << "-gen                    Generate the JSON file\n";
    std::cout << "-u <URL> -h <HEADER>    Send JSON data as HTTP POST request\n";
    std::cout << "<path_to_output.json>   Specify the output path for the JSON file (full path)\n\n";
}

// Function to generate json
void generateJson(const std::string& filePath) {
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

// Callback function for CURLOPT_WRITEFUNCTION
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output) {
    size_t totalSize = size * nmemb;
    output->append((char*)contents, totalSize);
    return totalSize;
}

// Function to send HTTP POST request
void sendHttpPostRequest(const std::string& url, const std::string& header, const std::string& filePath) {
    // Initialize libcurl
    CURL* curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Error initializing libcurl." << std::endl;
        exit(1);
    }

    // Set the target URL
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Set the HTTP header
    struct curl_slist* headers = NULL;
    headers = curl_slist_append(headers, ("X-Luna-API: " + header).c_str());
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    // Set the POST data
    std::ifstream inFile(filePath);
    std::string postData;
    if (inFile.is_open()) {
        inFile.seekg(0, std::ios::end);
        postData.reserve(inFile.tellg());
        inFile.seekg(0, std::ios::beg);
        postData.assign((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        inFile.close();
    } else {
        std::cerr << "Error reading " << filePath << "." << std::endl;
        curl_easy_cleanup(curl);
        exit(1);
    }
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, postData.c_str());

    // Set the write callback function to capture the response
    std::string responseData;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);

    // Perform the HTTP request
    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        std::cerr << "Error performing HTTP request: " << curl_easy_strerror(res) << std::endl;
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
        exit(1);
    }

    // Clean up
    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    // Display the response data
    std::cout << "HTTP POST request sent to " << url << " with header " << header << std::endl;
    std::cout << "Response: " << responseData << std::endl;
}

int main(int argc, char *argv[]) {
    // Check for version flag
    if ((argc == 2 && (std::string(argv[1]) == "-v" || std::string(argv[1]) == "--version"))) {
        printVersion();
        return 0;
    }

    // Check for help flag
    if ((argc == 2 && (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help"))) {
        printHelp();
        return 0;
    }

    // Check for generate JSON flag
    if (argc == 2 && std::string(argv[1]) == "-gen") {
        // Default file path
        std::string filePath = "data.json";

        // Check if a filepath is provided as a command-line argument
        if (argc > 2) {
            filePath = argv[2];
        }

        // Generate JSON
        generateJson(filePath);

        return 0;
    }

    // Check for HTTP POST request flag
    if (argc == 5 && std::string(argv[1]) == "-u" && std::string(argv[3]) == "-h") {
        std::string url = argv[2];
        std::string header = argv[4];

        // Default file path
        std::string filePath = "data.json";

        // Check if a filepath is provided as a command-line argument
        if (argc > 5) {
            filePath = argv[5];
        }

        // Read JSON from file
        std::ifstream inFile(filePath);
        json jsonData;
        if (inFile.is_open()) {
            inFile >> jsonData;
            inFile.close();

            // Send HTTP POST request
            sendHttpPostRequest(url, header, filePath);
        } else {
            std::cerr << "Error reading " << filePath << "." << std::endl;
            exit(1);
        }

        return 0;
    }

    // Invalid command-line arguments
    std::cerr << "Invalid command-line arguments. Use '-v' for version, '-gen' for JSON generation, or '-u <URL> -h <HEADER>' for HTTP POST request." << std::endl;
    exit(1);
}
