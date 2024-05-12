cmake_minimum_required(VERSION 3.14)

project(
    scene
    VERSION 0.1
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/public
    ${CMAKE_CURRENT_SOURCE_DIR}/public/component
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
    ${CMAKE_CURRENT_SOURCE_DIR}/private/component
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    core
    opengl
    event
PUBLIC
)

add_dependencies(${PROJECT_NAME}
    core
    opengl
    event
)
