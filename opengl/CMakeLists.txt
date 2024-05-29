cmake_minimum_required(VERSION 3.14)

project(
    opengl
    VERSION 0.1
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(Dependency.cmake)

file(GLOB_RECURSE SOURCE_FILES CONFIGURE_DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/private/*.cpp)
add_library(${PROJECT_NAME} ${SOURCE_FILES})

target_compile_definitions(${PROJECT_NAME}
PRIVATE
    WINDOW_INIT_WIDTH=1600
    WINDOW_INIT_HEIGHT=900
    WINDOW_NAME="Graphics"
    SHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shader/"
)

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${EXTERNAL_INCLUDE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/public
PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/private
)

target_link_directories(${PROJECT_NAME}
PUBLIC
    ${EXTERNAL_LIBRARY_DIR}
)

target_link_libraries(${PROJECT_NAME}
PRIVATE
    ${EXTERNAL_LIBRARYS}
PUBLIC
    core
)

add_dependencies(${PROJECT_NAME}
    ${EXTERNAL_PROJECTS}
    core
)
