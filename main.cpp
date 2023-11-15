#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "menu/menuFunctions.h"

int main(int argc, char* argv[]) {
    
    // help menu or version info
    if (argc == 2) {
        std::string currentArg = argv[1];
        std::transform(currentArg.begin(), currentArg.end(), currentArg.begin(), ::tolower);
        
        // help menu
        if (currentArg == "-h" || currentArg == "--help") {
            printHelp();
            return 0;
        }
        
        // version info
        else if (currentArg == "-v" || currentArg == "--version") {
            printVersion();
            return 0;
        }

        // fail notify
        else {
            std::cerr << "Error: Missing value for -u or --url flag." << std::endl;
            return 1;
        }
    }
    
    else if (argc == 7) {
        std::string apiEndpoint;
        std::string apiHeader;
        std::string jsonData;

        for (int i = 1; i < argc; ++i) {
            std::string currentArg = argv[i];
            std::transform(currentArg.begin(), currentArg.end(), currentArg.begin(), ::tolower);

            if (currentArg == "-u" || currentArg == "--url") {
                if (i + 1 < argc) {
                    apiEndpoint = argv[i + 1];
                } else {
                    std::cerr << "Error: Missing value for -u or --url flag." << std::endl;
                    return 1;
                }
            } else if (currentArg == "-h" || currentArg == "--header") {
                if (i + 1 < argc) {
                    apiHeader = argv[i + 1];
                } else {
                    std::cerr << "Error: Missing value for -h or --header flag." << std::endl;
                    return 1;
                }
            } else if (currentArg == "-d" || currentArg == "--data") {
                if (i + 1 < argc) {
                    jsonData = argv[i + 1];
                } else {
                    std::cerr << "Error: Missing value for -d or --data flag." << std::endl;
                    return 1;
                }
            }
        }

        // Now you have the values assigned to apiEndpoint, apiHeader, and jsonData
        std::cout << "API Endpoint: " << apiEndpoint << std::endl;
        std::cout << "API Header: " << apiHeader << std::endl;
        std::cout << "JSON Data: " << jsonData << std::endl;

        // Example: Use the parsed values in your application logic
        // Replace this with the actual logic you want to execute

        // For demonstration purposes, just printing a message
        std::cout << "Executing the rest of the code..." << std::endl;

    } else {
        std::cerr << "Usage: " << argv[0] << " -u|--url <API_ENDPOINT> -h|--header <API_HEADER> -d|--data <JSON_DATA>" << std::endl;
        return 1;
    }

    return 0;
}
