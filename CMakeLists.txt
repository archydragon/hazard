# Main CMake configuration for this environment
cmake_minimum_required(VERSION 3.17)
set(CMAKE_TOOLCHAIN_FILE ${CMAKE_SOURCE_DIR}/tools/vcpkg/scripts/buildsystems/vcpkg.cmake)
set(VCPKG_TARGET_TRIPLET x86-windows)

# Main project configuration
set(PROJECT hazard)
project(${PROJECT}
    VERSION 0.1.0
    LANGUAGES CXX C)
set(CMAKE_CXX_STANDARD 17)

add_definitions(-D_DLL -DIMGUI_IMPL_OPENGL_LOADER_GLAD)

add_executable(${PROJECT} src/main.cpp src/Config.cpp src/Config.h src/ui/UI.cpp src/ui/UI.h src/engine/Camera.cpp src/engine/Camera.h src/engine/Scene.cpp src/engine/Scene.h src/engine/Shader.h src/engine/Shader.cpp)

# Add CMakeLists.txt for 3rd party libs
add_subdirectory(3rdparty)

# Libraries bundled under 3rdparty dir
target_link_libraries(${PROJECT} PRIVATE
    glad
    imgui)

# Libraries installed through vcpkg
find_package(glfw3 REQUIRED)
find_package(nlohmann_json REQUIRED)
target_link_libraries(${PROJECT} PRIVATE
    opengl32
    glfw3
    nlohmann_json::nlohmann_json)
