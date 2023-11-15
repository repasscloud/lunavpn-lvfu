cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 17)

project(LUNVAVPN-LVFU)

find_package(CURL REQUIRED)
include_directories(${CURL_INCLUDE_DIR})

include_directories(jsondata)

add_executable(lvpnfu main.cpp
                      jsondata/generateJsonFunctions.cpp
                      menu/menuFunctions.cpp
                      http/apiFunctions.cpp)

target_link_libraries(lvpnfu ${CURL_LIBRARIES})

# Specify the output directory for the binary during build
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Optionally, you can add more specific include directories if needed
# include_directories(jsondata/nlohmann)

# Other configurations...

# Specify the output directory for the binary
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/out)

# Add a custom target for cleaning up build artifacts
add_custom_target(clean-all
    COMMAND ${CMAKE_BUILD_TOOL} clean
    COMMAND ${CMAKE_COMMAND} -P ${CMAKE_BINARY_DIR}/cmake/clean-all.cmake
)

# Create a clean-all.cmake script
file(WRITE ${CMAKE_BINARY_DIR}/cmake/clean-all.cmake
    "FILE(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/out)\n"
    "FILE(REMOVE_RECURSE ${CMAKE_BINARY_DIR}/CMakeFiles)\n"
    "FILE(REMOVE ${CMAKE_BINARY_DIR}/CMakeCache.txt)\n"
    "FILE(REMOVE ${CMAKE_BINARY_DIR}/Makefile)\n"
)

# Add clean-all as a dependency of clean if clean exists
if(TARGET clean)
    add_dependencies(clean clean-all)
endif()
