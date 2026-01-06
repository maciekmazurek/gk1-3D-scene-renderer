cmake_minimum_required(VERSION 3.15)
project("gk1-3D-scene-renderer" LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)
find_package(glm CONFIG REQUIRED)

set(PROJECT_SOURCES
    src/app/main.cpp
    src/core/Application.cpp
    src/core/Window.cpp
    src/rendering/Renderer.cpp
    src/rendering/ShaderProgram.cpp
    src/rendering/TetrahedronMesh.cpp
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})
target_include_directories(${PROJECT_NAME} PRIVATE include)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad glfw OpenGL::GL glm::glm)

source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR} FILES ${PROJECT_SOURCES})