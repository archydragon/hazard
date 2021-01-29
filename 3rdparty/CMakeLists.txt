# Common file for all 3rd party dependencies under this directory.
cmake_minimum_required(VERSION 3.17)

# glad
set(DEP_GLAD glad)
add_library(${DEP_GLAD} STATIC
    ${DEP_GLAD}/src/glad.c)
target_include_directories(${DEP_GLAD} PUBLIC
    ${DEP_GLAD}/include)
