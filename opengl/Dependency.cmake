include(ExternalProject)
include(FetchContent)

set(EXTERNAL_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_INSTALL_DIR}/include)
set(EXTERNAL_LIBRARY_DIR ${EXTERNAL_INSTALL_DIR}/lib)

ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.4"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
        -DGLFW_BUILD_EXAMPLES=OFF
        -DGLFW_BUILD_TESTS=OFF
        -DGLFW_BUILD_DOCS=OFF
    TEST_COMMAND ""
    )
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep_glfw)
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS} glfw3)

# Generate GLAD using Python
ExternalProject_Add(
    dep_glad_generator
    GIT_REPOSITORY "https://github.com/Dav1dde/glad.git"
    GIT_TAG "v0.1.36"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ${CMAKE_COMMAND} -E env python -m glad --out-path=${EXTERNAL_INSTALL_DIR} --api=gl=3.3 --generator=c --spec=gl --no-loader
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
    TEST_COMMAND ""
    )
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep_glad_generator)

# Add GLAD source files to be compiled with the project
set(GLAD_INCLUDE_DIR ${EXTERNAL_INSTALL_DIR}/include)
set(GLAD_SOURCE ${EXTERNAL_INSTALL_DIR}/src/glad.c)

ExternalProject_Add(
    dep_stb
    GIT_REPOSITORY "https://github.com/nothings/stb"
    GIT_TAG "master"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CONFIGURE_COMMAND ""
    BUILD_COMMAND ""
    TEST_COMMAND ""
    INSTALL_COMMAND ${CMAKE_COMMAND} -E copy
      ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image.h
      ${EXTERNAL_INCLUDE_DIR}/stb/stb_image.h
      && ${CMAKE_COMMAND} -E copy
      ${PROJECT_BINARY_DIR}/dep_stb-prefix/src/dep_stb/stb_image_write.h
      ${EXTERNAL_INCLUDE_DIR}/stb/stb_image_write.h
    )
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep_stb)

ExternalProject_Add(
  dep_assimp
  GIT_REPOSITORY "https://github.com/assimp/assimp"
  GIT_TAG "v5.4.3"
  GIT_SHALLOW 1
  UPDATE_COMMAND ""
  PATCH_COMMAND ""
  CMAKE_ARGS
      -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
      -DBUILD_SHARED_LIBS=OFF
      -DASSIMP_BUILD_ASSIMP_TOOLS=OFF
      -DASSIMP_BUILD_TESTS=OFF
      -DASSIMP_INJECT_DEBUG_POSTFIX=OFF
      -DASSIMP_BUILD_ZLIB=ON
  TEST_COMMAND ""
  )
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep_assimp)
if (WIN32)
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS}
  assimp-vc143-mt
  zlibstatic$<$<CONFIG:Debug>:d>
)
else ()
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS}
  assimp
  zlibstatic
)
endif()
