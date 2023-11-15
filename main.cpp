#include <arpa/inet.h>
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include "jsondata/generateJsonFunctions.h"
#include "menu/menuFunctions.h"
#include "http/apiFunctions.h"

int main(int argc, char *argv[]) {
    
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
    
    // json output generation
    else if (argc == 3) {
        std::string currentArg = argv[1];
        std::transform(currentArg.begin(), currentArg.end(), currentArg.begin(), ::tolower);

        std::string valueArg = argv[2];
        std::transform(valueArg.begin(), valueArg.end(), valueArg.begin(), ::tolower);

        // set filepath for output/input
        std::string filePath = valueArg;

        // generate switch
        if (currentArg == "-g" || currentArg == "--generate-json") {
            
            // Generate JSON (or replace existing file)
            generateJson(filePath);

            return 0;
        }
        else {
            std::cerr << "Invalid switch or filepath: " << filePath << "." << std::endl;
            return 1;
        }
    }

    // run HTTP POST request
    else if (argc == 7) {
        std::string apiEndpoint;
        std::string apiHeader;
        std::string jsonDataFile;

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
            } else if (currentArg == "-d" || currentArg == "--data-file") {
                if (i + 1 < argc) {
                    jsonDataFile = argv[i + 1];
                } else {
                    std::cerr << "Error: Missing value for -d or --data-file flag." << std::endl;
                    return 1;
                }
            }
        }
    }

    // Invalid command-line arguments
    else {
        std::cerr << "Invalid command-line arguments. Use '-h' for help menu." << std::endl;
        exit(1);
    }
}
