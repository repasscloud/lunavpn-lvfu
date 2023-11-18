// squid/squidFunctions.h
#ifndef SQUID_FUNCTIONS_H
#define SQUID_FUNCTIONS_H

#include <vector>
#include <string> // Include the necessary header for std::string

bool isSafeInput(const std::string& input);
void generateDictionary(std::vector<std::string>& words);
std::string generateUsername();
std::string generatePassword();
#endif