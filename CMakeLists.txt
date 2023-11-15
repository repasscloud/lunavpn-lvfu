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

# Specify the installation directory for the executable
# This sets the installation path to ./out when you run 'make install'
set(CMAKE_INSTALL_PREFIX ${CMAKE_BINARY_DIR}/out)

# Specify the installation target
install(TARGETS lvpnfu DESTINATION .)
