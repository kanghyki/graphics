include(ExternalProject)

set(EXTERNAL_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_INSTALL_DIR}/include)
set(EXTERNAL_LIBRARY_DIR ${EXTERNAL_INSTALL_DIR}/lib)

ExternalProject_Add(dep-spdlog
  GIT_REPOSITORY "https://github.com/gabime/spdlog.git"
  GIT_TAG "v1.x"
  CMAKE_ARGS
    -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
)
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep-spdlog)
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS} spdlog$<$<CONFIG:Debug>:d>)

ExternalProject_Add(dep_glm
  GIT_REPOSITORY "https://github.com/g-truc/glm"
  GIT_TAG "0.9.9.8"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CONFIGURE_COMMAND ""
  BUILD_COMMAND ""
  TEST_COMMAND ""
  INSTALL_COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_BINARY_DIR}/dep_glm-prefix/src/dep_glm/glm
    ${EXTERNAL_INCLUDE_DIR}/glm
)
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep_glm)
