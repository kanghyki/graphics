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

add_executable(${PROJECT_NAME} main.cpp)

list(APPEND libraries
    core
    opengl
    scene
    event
)
foreach(lib IN LISTS libraries)
    message(STATUS "${lib}")
    add_subdirectory(${lib})
endforeach()

target_link_libraries(${PROJECT_NAME}
PRIVATE
    ${libraries}
)

add_dependencies(${PROJECT_NAME} ${libraries})
