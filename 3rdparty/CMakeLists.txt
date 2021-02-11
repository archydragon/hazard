# Common file for all 3rd party dependencies under this directory.
cmake_minimum_required(VERSION 3.17)

# glad
set(DEP_GLAD glad)
add_library(${DEP_GLAD} STATIC
    ${DEP_GLAD}/src/glad.c)
target_include_directories(${DEP_GLAD} PUBLIC
    ${DEP_GLAD}/include)

# imgui
set(DEP_IMGUI imgui)
include_directories(${DEP_GLAD}/include) # needed because Imgui depends on GLAD loader
add_library(${DEP_IMGUI} STATIC
    ${DEP_IMGUI}/imgui.cpp
    ${DEP_IMGUI}/imgui_draw.cpp
    ${DEP_IMGUI}/imgui_tables.cpp
    ${DEP_IMGUI}/imgui_widgets.cpp
    ${DEP_IMGUI}/backends/imgui_impl_glfw.cpp
    ${DEP_IMGUI}/backends/imgui_impl_opengl3.cpp)
target_include_directories(${DEP_IMGUI} PUBLIC
    ${DEP_IMGUI})

# ImGuiFileDialog
set(DEP_FILEDIALOG ImGuiFileDialog)
include_directories(${DEP_IMGUI}) # include original Imgui
add_library(${DEP_FILEDIALOG} STATIC
    ${DEP_FILEDIALOG}/ImGuiFileDialog.cpp)
target_include_directories(${DEP_FILEDIALOG} PUBLIC
    ${DEP_FILEDIALOG})

# implot
set(DEP_IMPLOT implot)
include_directories(${DEP_IMGUI})
add_library(${DEP_IMPLOT} STATIC
    ${DEP_IMPLOT}/implot.cpp
    ${DEP_IMPLOT}/implot_items.cpp)
target_include_directories(${DEP_IMPLOT} PUBLIC
    ${DEP_IMPLOT})
