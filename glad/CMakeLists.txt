cmake_minimum_required(VERSION 3.0)
project(glad)

set(CMAKE_CXX_STANDARD 11)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_C_FLAGS ${CMAKE_C_FLAGS} -fPIC)
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -fPIC)
endif()

include_directories(include)
add_library(glad STATIC ${CMAKE_CURRENT_SOURCE_DIR}/include/glad/glad.h ${CMAKE_CURRENT_SOURCE_DIR}/include/KHR/khrplatform.h ${CMAKE_CURRENT_SOURCE_DIR}/src/glad.c)
if(UNIX)
    target_link_libraries(glad dl)
endif()