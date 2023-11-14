#!/bin/sh
url="https://github.com/nlohmann/json/releases/latest"
latest_tag=$(curl -sSL "$url" | grep -o 'tag/v[0-9][^"]*' | cut -d'/' -f2 | head -n1)

if [ -n "$latest_tag" ]; then
    folder_name="nlohmann"

    # Check if the folder exists
    if [ -d "$folder_name" ]; then
        # Remove the folder recursively
        echo "Removing folder $folder_name..."
        rm -r "$folder_name"
        echo "Folder $folder_name removed successfully."
    else
        echo "Folder $folder_name does not exist."
    fi

    echo "Latest tag: $latest_tag"

    # Download the release
    release_url="https://github.com/nlohmann/json/releases/download/${latest_tag}/json.hpp"
    echo "Downloading release from: $release_url"
    curl -LO "$release_url"

    # Create folder structure and move the file
    folder_path="nlohmann/json"
    mkdir -p "$folder_path"
    mv "json.hpp" "$folder_path/"
    
    echo "File 'json.hpp' moved to '$folder_path/'"

    # Update JSON for Modern C++ version number
    file_path="lvfu.cpp"

    # Check if the file exists
    if [ -e "$file_path" ]; then
        # Use sed to perform the replacement
        sed -i "s/__VERSION__/$latest_tag/g" "$file_path"
        echo "Replacement completed successfully in $file_path"
    else
        echo "File $file_path not found."
    fi

    make clean

    make

    ./out/lvfu -v
else
    echo "Unable to retrieve the latest tag."
fi
