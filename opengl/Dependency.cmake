include(ExternalProject)

set(EXTERNAL_INSTALL_DIR ${PROJECT_BINARY_DIR}/install)
set(EXTERNAL_INCLUDE_DIR ${EXTERNAL_INSTALL_DIR}/include)
set(EXTERNAL_LIBRARY_DIR ${EXTERNAL_INSTALL_DIR}/lib)

ExternalProject_Add(
    dep_glfw
    GIT_REPOSITORY "https://github.com/glfw/glfw.git"
    GIT_TAG "3.3.2"
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

ExternalProject_Add(
    dep_glad
    GIT_REPOSITORY "https://github.com/Dav1dde/glad"
    GIT_TAG "v0.1.34"
    GIT_SHALLOW 1
    UPDATE_COMMAND ""
    PATCH_COMMAND ""
    CMAKE_ARGS
        -DCMAKE_INSTALL_PREFIX=${EXTERNAL_INSTALL_DIR}
        -DGLAD_INSTALL=ON
    TEST_COMMAND ""
    )
set(EXTERNAL_PROJECTS ${EXTERNAL_PROJECTS} dep_glad)
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS} glad)

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


# assimp
ExternalProject_Add(
  dep_assimp
  GIT_REPOSITORY "https://github.com/assimp/assimp"
  GIT_TAG "v5.0.1"
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

if (WINDOWS)

set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS}
  assimp-vc143-mt$<$<CONFIG:Debug>:d>
  zlibstatic$<$<CONFIG:Debug>:d>
  IrrXML$<$<CONFIG:Debug>:d>
)
else ()
set(EXTERNAL_LIBRARYS ${EXTERNAL_LIBRARYS}
  assimp
  zlibstatic
  IrrXML
)
endif()
