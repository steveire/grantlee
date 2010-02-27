
# The use file for Grantlee, distributed along with Grantlee
include(CMakeImportBuildSettings)
cmake_import_build_settings(${Grantlee_BUILD_SETTINGS_FILE})

# Set up the include directories and link directories
include_directories(${Grantlee_INCLUDE_DIRS})
link_directories(${Grantlee_LIBRARY_DIRS})

# Set up the deps needed to use Grantlee
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# Add the Grantlee modules directory to the CMake module path
set(CMAKE_MODULE_PATH ${Grantlee_PLUGIN_DIR}/cmake ${CMAKE_MODULE_PATH})

macro(GRANTLEE_ADD_PLUGIN pluginname)
  foreach(_filename ${ARGN})
    set(_headers ${_headers} ${_filename}.h)
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach(_filename ${ARGN})
  qt4_wrap_cpp(_plugin_moc_srcs ${_headers})
  add_library(${pluginname} SHARED ${_sources} ${_plugin_moc_srcs})
  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
  )
endmacro(GRANTLEE_ADD_PLUGIN)
