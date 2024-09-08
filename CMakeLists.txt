#create le project
cmake_minimum_required(VERSION 3.15)
project(moop VERSION 0.0.0 LANGUAGES C CXX)
set (CMAKE_CXX_STANDARD 20)

#adds all the needed C, CPP, HPP and H files
file(GLOB_RECURSE SOURCES
    src/*.cpp
    src/*.c
    src/*.hpp
    src/*.h
    glad/include/glad/glad.h
    glad/include/KHR/khrplatform.h
    glad/src/glad.c
)

add_executable(moop ${SOURCES})

#libs
target_include_directories(moop PUBLIC 
    ${PROJECT_SOURCE_DIR}/glfw/include 
    ${PROJECT_SOURCE_DIR}/glad/include/glad
    ${PROJECT_SOURCE_DIR}/glad/include/KHR
    ${PROJECT_SOURCE_DIR}/msfl/include
)

if (WIN32) #Windows
    find_package(OpenGL REQUIRED)

    add_compile_definitions(WIN32)
    target_link_libraries(moop PUBLIC
        ${PROJECT_SOURCE_DIR}/glfw/glfw3dll.lib
        ${PROJECT_SOURCE_DIR}/msfl/win64/dynamic/msfl.lib
        OpenGL::GL
    )
else (WIN32) #UNIX / Mac
    add_compile_definitions(MAC)
    target_link_libraries(moop PUBLIC
        ${PROJECT_SOURCE_DIR}/glfw/lib-arm64/libglfw3.a
        ${PROJECT_SOURCE_DIR}/msfl/arm64/static/libmsfl.a
        "-framework Cocoa"
        "-framework OpenGL"
        "-framework IOKit"
    )
endif (WIN32)

#TODO: msfl