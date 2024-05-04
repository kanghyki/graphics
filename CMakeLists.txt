cmake_minimum_required(VERSION 3.14)

project(
  main 
  VERSION 0.1
  LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(${PROJECT_NAME} main.cpp)

add_subdirectory(core)

target_link_libraries(${PROJECT_NAME}
PRIVATE
  core
)

add_dependencies(${PROJECT_NAME} core)
