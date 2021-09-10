cmake_minimum_required(VERSION 3.20)
project(SDL2_GLAD_GLM_Triangles)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)
set(LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(SDL2-2.0.16)
add_subdirectory(glm)
add_subdirectory(glad)

include_directories(
        SDL2-2.0.16/include
        glad/include
        glm)

add_executable(SDL2_GLAD_GLM_Triangles main.cpp)
target_link_libraries(SDL2_GLAD_GLM_Triangles glad SDL2)
