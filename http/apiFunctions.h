// apiFunctions.h
#ifndef API_FUNCTIONS_H
#define API_FUNCTIONS_H

#include <string>

// Callback function for CURLOPT_WRITEFUNCTION
size_t writeCallback(void* contents, size_t size, size_t nmemb, std::string* output);

// Function to send HTTP POST request
void sendHttpPostRequest(const std::string& url, const std::string& header, const std::string& filePath);

#endif // API_FUNCTIONS_H
