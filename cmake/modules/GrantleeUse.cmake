
# Set up the include directories and link directories
include_directories(${Grantlee_INCLUDE_DIRS})

# Set up the deps needed to use Grantlee
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

include(CMakeParseArguments)

# Add the Grantlee modules directory to the CMake module path
set(CMAKE_MODULE_PATH ${Grantlee_PLUGIN_DIR}/cmake ${CMAKE_MODULE_PATH})

macro(GRANTLEE_ADD_PLUGIN pluginname)
  set(options)
  set(oneValueArgs TAGS FILTERS)
  set(multiValueArgs)

  cmake_parse_arguments(_PLUGIN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  foreach(_filename ${_PLUGIN_UNPARSED_ARGUMENTS})
    get_source_file_property(_skip ${_filename}.h SKIP_AUTOMOC)
    if (NOT _skip)
      set(_headers ${_headers} ${_filename}.h)
    endif()
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach()
  foreach(_filename ${_PLUGIN_TAGS})
    set(_headers ${_headers} ${_filename}.h)
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach()
  foreach(_filename ${_PLUGIN_FILTERS})
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach()

  qt4_wrap_cpp(_plugin_moc_srcs ${_headers})

  add_library(${pluginname} MODULE ${_sources} ${_plugin_moc_srcs})

  foreach(file ${_sources})
    set(_sources_FULLPATHS ${_sources_FULLPATHS} ${CMAKE_CURRENT_SOURCE_DIR}/${file})
  endforeach()
  set_property(GLOBAL APPEND PROPERTY SOURCE_LIST
    ${_sources_FULLPATHS}
  )

  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
  )
  target_link_libraries(${pluginname}
    grantlee_core
  )
endmacro(GRANTLEE_ADD_PLUGIN)

if (MINGW)
  # http://mail.kde.org/pipermail/kde-windows/2007-December/001692.html
  # http://lists.trolltech.com/pipermail/qt-interest/2009-July/009829.html
  # qt is always compiled with QT_NO_DEBUG under mingw,
  # so we need to compile stuff linked against it
  # the same or otherwise QPluginLoader rejects plugins
  add_definitions(-DQT_NO_DEBUG)
endif ()
