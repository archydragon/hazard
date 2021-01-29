# Main CMake configuration for this environment
cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(_VCPKG_INSTALLED_DIR ${CMAKE_SOURCE_DIR}/vcpkg_installed)
set(VCPKG_TARGET_TRIPLET x86-windows)

# Main project configuration
set(PROJECT hazard)
project(${PROJECT}
    VERSION 0.1.0
    LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)

add_executable(${PROJECT} src/main.cpp)

# Add CMakeLists.txt for 3rd party libs
add_subdirectory(3rdparty)

# Libraries bundled under 3rdparty dir
target_link_libraries(${PROJECT} PRIVATE
    glad)

# Libraries installed through vcpkg
target_link_libraries(${PROJECT} PRIVATE
    glfw3)
