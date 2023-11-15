// apiFunctions.cpp
#include "apiFunctions.h"
#include <iostream>
#include <fstream>
#include <cstring>
#include <curl/curl.h>

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
    std::cout << "Response: \n" << responseData << std::endl;
}
