// menu/menuFunctions.cpp
#include <iostream>

// Function to print help info
void printHelp() {
    std::cout << "###################################################\n";
    std::cout << "#              LunaVPN fu vX.X.X                  #\n";
    std::cout << "###################################################\n\n";
    std::cout << "Welcome to LunaVPN fu - Your Functioning Unit for networking needs!\n";
    std::cout << "Copyright © RePass Cloud Pty Ltd 2023\n\n";
    std::cout << "Usage: LunaVPN_fu [-v] | [-h] | [-g <path_to_output.json>]\n\n";
    std::cout << "  -v|--version                                Display version information\n";
    std::cout << "  -h|--help                                   Display help\n";
    std::cout << "  -g|--generate-json <path_to_output.json>    Generate the JSON file\n\n";
    std::cout << "Usage: LunaVPN_fu [-u <URL> -h <HEADER> -d <path_to_input.json>]\n\n";
    std::cout << "HTTP POST request\n";
    std::cout << "  -u|--url <URL>                        Specify the URL for HTTP POST request\n";
    std::cout << "  -h|-header <HEADER>                   Specify the header for HTTP POST request\n";
    std::cout << "  -d|--data-file <path_to_input.json>   Specify the data file for HTTP POST request\n\n";
}


// Function to print version information
void printVersion() {
    std::cout << "###################################################\n";
    std::cout << "#              LunaVPN fu vX.X.X                  #\n";
    std::cout << "###################################################\n\n";
    std::cout << "Welcome to LunaVPN fu - Your Functioning Unit for networking needs!\n";
    std::cout << "Copyright © RePass Cloud Pty Ltd 2023\n\n";
    std::cout << "LunaVPN fu v1.2.0\n";
    std::cout << "Using JSON for Modern C++ ___NLOHMANN_VERSION___\n\n";
}