include(ExternalProject)

set(EXTERNAL_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_INSTALL_DIR}/include)
set(EXTERNAL_LIBRARY_DIR ${EXTERNAL_INSTALL_DIR}/lib)

add_library(
    imgui
    imgui/imgui_draw.cpp
    imgui/imgui_tables.cpp
    imgui/imgui_widgets.cpp
    imgui/imgui.cpp
    imgui/imgui_impl_glfw.cpp
    imgui/imgui_impl_opengl3.cpp
    )
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_INCLUDE_DIR} ${CMAKE_CURRENT_SOURCE_DIR}/imgui)
target_link_libraries(imgui
PRIVATE
    opengl
)
add_dependencies(imgui opengl)

set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} imgui)
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS} imgui)