#include <arpa/inet.h>
#include <curl/curl.h>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstdlib>
#include <sstream>
#include "jsondata/generateJsonFunctions.h"
#include "menu/menuFunctions.h"
#include "http/apiFunctions.h"
#include "squid/squidFunctions.h"

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

        std::string squidPath = filePath;

        // Check if there is a trailing "/"
        if (!squidPath.empty() && squidPath.back() == '/') {
            // Remove the trailing "/"
            squidPath.pop_back();
        }

        // set squid.creds path
        std::string squidCredsPath = squidPath + "/squid.creds";

        // set squid htpasswd
        std::string htpasswdPath = squidPath + "/htpasswd";

        // generate switch
        if (currentArg == "-gj" || currentArg == "--generate-json") {
            // Generate JSON (or replace existing file)
            generateJson(filePath);

            return 0;
        }
        else if (currentArg == "-gs" || currentArg == "--generate-squid") {
            

            // Seed the random number generator
            srand(static_cast<unsigned int>(time(0)));

            // Generate Squid Proxy username and password
            std::string username = generateUsername();
            std::string password = generatePassword();

            // check username and password is safe
            if (!isSafeInput(username) || !isSafeInput(password)) {
                std::cerr << "Invalid input. Only alphanumeric characters are allowed.\n";
                return 1;
            }

            // Construct the htpasswd command
            std::ostringstream commandStream;
            commandStream << "htpasswd -cb " << htpasswdPath << " " << username << " " << password;

            std::string command = commandStream.str();

            int result = std::system(command.c_str());

            if (result == 0) {
                std::cout << "htpasswd command executed successfully.\n";
            } else {
                std::cerr << "Error executing htpasswd command.\n";
            }

            // Write to the file without trailing newline
            std::ofstream outFile(squidCredsPath, std::ios::out | std::ios::trunc);
            if (outFile.is_open()) {
                outFile << username << ":" << password;
                outFile.close();
                std::cout << "Credentials written to " << squidCredsPath << std::endl;
            } else {
                std::cerr << "Error opening file for writing!" << std::endl;
                return 1;
            }

            return 0;
        }
        else {
            std::cerr << "Invalid switch or filepath: " << squidCredsPath << "." << std::endl;
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
