get_filename_component(CURRENT_CMAKE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
list(APPEND CMAKE_MODULE_PATH ${CURRENT_CMAKE_DIR})

#include(CMakeFindDependencyMacro) # use find_package instead

# NOTE: some packages may be optional (platform-specific, etc.)
# find_package(... REQUIRED)
find_package(chromium_base REQUIRED)
# see https://doc.magnum.graphics/corrade/corrade-cmake.html#corrade-cmake-subproject
find_package(Corrade REQUIRED PluginManager)
find_package(Cling)

list(REMOVE_AT CMAKE_MODULE_PATH -1)

if(NOT TARGET CONAN_PKG::squarets)
  message(FATAL_ERROR "Use squarets from conan")
endif()
set(squarets_LIB CONAN_PKG::squarets)
# conan package has '/include' dir
set(squarets_HEADER_DIR
  ${CONAN_SQUARETS_ROOT}/include
)
if(squarets_LOCAL_BUILD)
  # name of created target
  set(squarets_LIB squarets)
  # no '/include' dir on local build
  set(squarets_HEADER_DIR
    ${CONAN_SQUARETS_ROOT}
  )
else()
  # uses Config.cmake or a -config.cmake file
  # see https://gitlab.kitware.com/cmake/community/wikis/doc/tutorials/How-to-create-a-ProjectConfig.cmake-file
  # BELOW MUST BE EQUAL TO find_package(... CONFIG REQUIRED)
  # NOTE: find_package(CONFIG) not supported with EMSCRIPTEN, so use include()
  include(${CMAKE_CURRENT_LIST_DIR}/cmake/squarets-config.cmake)
endif(squarets_LOCAL_BUILD)
message(STATUS "squarets_HEADER_DIR=${squarets_HEADER_DIR}")
