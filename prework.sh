#!/bin/bash

# openssl_url="https://api.github.com/repos/openssl/openssl/releases/latest"
# openssl_latest_tag=$(curl -sSL "$openssl_url" | jq -r '.tag_name' | grep -o 'openssl-[0-9][^"]*')

# lvpnfu_url="https://api.github.com/repos/repasscloud/lunavpn-lvfu/releases/latest"
# lvpnfu_latest_tag=$(curl -sSL "$lvpnfu_url" | grep -o 'tag/v[0-9][^"]*' | cut -d'/' -f2 | head -n1)
# curl -o ./out/lvpnfu https://github.com/repasscloud/lunavpn-lvfu/releases/download/$lvpnfu_latest_tag/lvpnfu

json_url="https://github.com/nlohmann/json/releases/latest"
json_latest_tag=$(curl -sSL "$json_url" | grep -o 'tag/v[0-9][^"]*' | cut -d'/' -f2 | head -n1)

# Nlohmann JSON
if [ -n "$json_latest_tag" ]; then
    json_folder_name="nlohmann"

    # Check if the folder exists
    if [ -d "$json_folder_name" ]; then
        # Remove the folder recursively
        echo "Removing folder $json_folder_name..."
        rm -r "$json_folder_name"
        echo "Folder $json_folder_name removed successfully."
    else
        echo "Folder $json_folder_name does not exist."
    fi

    echo "Nlohmann JSON Latest Tag: $json_latest_tag"

    # Download the release
    json_release_url="https://github.com/nlohmann/json/releases/download/${json_latest_tag}/json.hpp"
    echo "Downloading release from: $json_release_url"
    curl -LO "$json_release_url"

    # Create folder structure and move the file
    json_folder_path="jsondata/nlohmann"
    mkdir -p "$json_folder_path"
    mv "json.hpp" "$json_folder_path/"
    
    echo "File 'json.hpp' moved to '$json_folder_path/'"

    # Update JSON for Modern C++ version number
    json_file_path="menu/menuFunctions.cpp"
    json_temp_file="menu/menuFunctions_temp.cpp"

    # Check if the file exists
    if [ -e "$json_file_path" ]; then
        # Use sed to perform the replacement, using a temporary file
        sed "s/___NLOHMANN_VERSION___/${json_latest_tag//\//\\/}/g" "$json_file_path" > "$json_temp_file" && mv "$json_temp_file" "$json_file_path"
        echo "Replacement completed successfully in $json_file_path"
    else
        echo "File $json_file_path not found."
    fi
else
    echo "Unable to retrieve the latest tag for Nlohman JSON"
fi

# Read version_info file
VERSION_INFO=$(cat version_info)

echo $VERSION_INFO

# Search and replace in files
find . -type f -name '*' -exec sed -i "s/vX.X.X/v${VERSION_INFO}/g" {} +

# Add base64 library
# rm -rf cpp-base64
# git clone https://github.com/ReneNyffenegger/cpp-base64.git
# docker run --rm -v $(pwd)/cpp-base64:/cpp-base64 -w /cpp-base64 ubuntu bash -c "apt-get update && apt-get install -y g++ make && make base64-17.o"
# cp cpp-base64/base64.cpp jsondata/
# cp cpp-base64/base64.h jsondata/