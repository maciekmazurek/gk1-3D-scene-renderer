cmake_minimum_required (VERSION 3.15)
project ("gk1-3D-scene-renderer")

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

add_executable (${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad glfw OpenGL::GL)