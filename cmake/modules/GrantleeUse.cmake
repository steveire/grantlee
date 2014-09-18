
# Set up the include directories and link directories
include_directories(${Grantlee_INCLUDE_DIRS})

# Add the Grantlee modules directory to the CMake module path
set(CMAKE_MODULE_PATH ${Grantlee_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(CMakeParseArguments)

macro(GRANTLEE_ADD_PLUGIN pluginname)
  set(options)
  set(oneValueArgs)
  set(multiValueArgs TAGS FILTERS)

  cmake_parse_arguments(_PLUGIN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(_filename ${_PLUGIN_UNPARSED_ARGUMENTS})
    get_source_file_property(_skip ${_filename}.h SKIP_AUTOMOC)
    if (NOT _skip)
      list(APPEND _headers ${_filename}.h)
    endif()
    list(APPEND _sources ${_filename}.cpp)
  endforeach()
  foreach(_filename ${_PLUGIN_TAGS})
    list(APPEND _headers ${_filename}.h)
    list(APPEND _sources ${_filename}.cpp)
  endforeach()
  foreach(_filename ${_PLUGIN_FILTERS})
    list(APPEND _sources ${_filename}.cpp)
  endforeach()

  add_library(${pluginname} MODULE ${_sources} ${_plugin_moc_srcs})

  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
  )
  target_link_libraries(${pluginname}
    grantlee_core
  )
endmacro()
