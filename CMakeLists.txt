# Main CMake configuration for this environment
cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(_VCPKG_INSTALLED_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed)
set(VCPKG_TARGET_TRIPLET x86-windows)

# Main project configuration
project(hazard)
set(CMAKE_CXX_STANDARD 17)

add_executable(hazard main.cpp)

target_link_libraries(hazard PRIVATE
    glfw3)
