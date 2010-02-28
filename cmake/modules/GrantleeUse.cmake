
# Set up the include directories and link directories
include_directories(${Grantlee_INCLUDE_DIRS})
link_directories(${Grantlee_LIBRARY_DIRS})

# Set up the deps needed to use Grantlee
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# Add the Grantlee modules directory to the CMake module path
set(CMAKE_MODULE_PATH ${Grantlee_PLUGIN_DIR}/cmake ${CMAKE_MODULE_PATH})

include(ParseArguments)

macro(GRANTLEE_ADD_PLUGIN pluginname)
  parse_arguments(PLUGIN
      "TAGS;FILTERS"
      ""
      ${ARGN}
  )

  foreach(_filename ${PLUGIN_DEFAULT_ARGS})
    set(_headers ${_headers} ${_filename}.h)
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach(_filename ${PLUGIN_DEFAULT_ARGS})
  foreach(_filename ${PLUGIN_TAGS})
    set(_headers ${_headers} ${_filename}.h)
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach(_filename ${PLUGIN_TAGS})
  foreach(_filename ${PLUGIN_FILTERS})
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach(_filename ${PLUGIN_FILTERS})

  qt4_wrap_cpp(_plugin_moc_srcs ${_headers})
  add_library(${pluginname} SHARED ${_sources} ${_plugin_moc_srcs})
  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
  )
  target_link_libraries(${pluginname}
    grantlee_core_library
  )
endmacro(GRANTLEE_ADD_PLUGIN)
