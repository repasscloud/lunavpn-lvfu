// menu/menuFunctions.cpp
#include <iostream>

// Function to print help info
void printHelp() {
    std::cout << "###################################################\n";
    std::cout << "#              LunaVPN fu v1.1.0                  #\n";
    std::cout << "###################################################\n\n";
    std::cout << "Welcome to LunaVPN fu - Your Functioning Unit for networking needs!\n";
    std::cout << "Copyright © RePass Cloud Pty Ltd 2023\n\n";
    std::cout << "Usage: LunaVPN_fu [-v] | [-gen] | [-u <URL> -h <HEADER>] | [<path_to_output.json>]\n\n";
    std::cout << "-v|--version            Display version information\n";
    std::cout << "-h|--help               Display help\n";
    std::cout << "-gen                    Generate the JSON file\n";
    std::cout << "-u <URL> -h <HEADER>    Send JSON data as HTTP POST request\n";
    std::cout << "<path_to_output.json>   Specify the output path for the JSON file (full path)\n\n";
}


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