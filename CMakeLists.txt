cmake_minimum_required(VERSION 3.14)

project(
    main
    VERSION 0.1
    LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (APPLE)
    set(CMAKE_CXX_FLAGS "-framework Cocoa -framework IOKit -framework OpenGL -g")
endif()

if (MSVC)
    add_compile_options(/utf-8)
endif()

add_executable(${PROJECT_NAME}
    main.cpp
    level_test.cpp
)

# set_target_properties(${PROJECT_NAME}
# PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}"
# )

target_include_directories(${PROJECT_NAME}
PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/.
)

list(APPEND libraries
    core
    opengl
    scene
    event
    resource
)

# Editor
if (TRUE)
add_compile_definitions(
    EDITOR=true
)
list (APPEND libraries editor)
endif()

foreach(lib IN LISTS libraries)
    add_subdirectory(${lib})
endforeach()

target_link_libraries(${PROJECT_NAME}
PRIVATE
    ${libraries}
)

add_dependencies(${PROJECT_NAME} ${libraries})
