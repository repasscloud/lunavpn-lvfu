// squid/squidFunctions.cpp
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <string>
#include <cstring>
#include <sstream>

// Function to check if the input string consists of alphanumeric characters only
bool isSafeInput(const std::string& input) {
    return std::all_of(input.begin(), input.end(), ::isalnum);
}

// Function to generate the dictionary
void generateDictionary(std::vector<std::string>& words) {
    words.push_back("apple");
    words.push_back("banana");
    words.push_back("orange");
    words.push_back("chocolate");
    words.push_back("elephant");
    words.push_back("giraffe");
    words.push_back("sunflower");
    words.push_back("mountain");
    words.push_back("ocean");
    words.push_back("coffee");
    words.push_back("rainbow");
    words.push_back("butterfly");
    words.push_back("guitar");
    words.push_back("diamond");
    words.push_back("firefly");
    words.push_back("waterfall");
    words.push_back("whisper");
    words.push_back("harmony");
    words.push_back("serenity");
    words.push_back("laughter");
    words.push_back("puzzle");
    words.push_back("sunset");
    words.push_back("umbrella");
    words.push_back("vibrant");
    words.push_back("silhouette");
    words.push_back("lighthouse");
    words.push_back("illusion");
    words.push_back("tranquil");
    words.push_back("whimsical");
    words.push_back("enchanted");
    words.push_back("celestial");
    words.push_back("harmonious");
    words.push_back("effervescent");
    words.push_back("mystical");
    words.push_back("nostalgic");
    words.push_back("ephemeral");
    words.push_back("lullaby");
    words.push_back("radiance");
    words.push_back("sapphire");
}

// Function to generate username
std::string generateUsername() {
    // Dictionary of words
    std::vector<std::string> words;

    // Add words to the vector in the constructor
    generateDictionary(words);

    // Randomly select two words
    std::string word1 = words[rand() % words.size()];
    std::string word2 = words[rand() % words.size()];

    // Capitalize the first letter of the first word
    word1[0] = toupper(word1[0]);

    // Add a hyphen and a digit to the second word
    word2 += std::to_string(rand() % 10);

    // Combine the words to form the username
    std::string username = word1 + "-" + word2;
    return username;
}

// Function to generate password
std::string generatePassword() {
    const std::string charset = "ABCDEFGHKMNPQRSTWXYZabcdefghjkmnpqrstwxyz23456789";

    std::string password;
    srand(static_cast<unsigned int>(time(0)));

    // Generate a 12-character alphanumeric password
    for (int i = 0; i < 14; ++i) {
        password += charset[rand() % charset.length()];
    }

    return password;
}