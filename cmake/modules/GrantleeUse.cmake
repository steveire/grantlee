
# Set up the include directories and link directories
include_directories(${Grantlee_INCLUDE_DIRS})
link_directories(${Grantlee_LIBRARY_DIRS})

# Set up the deps needed to use Grantlee
find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

# Add the Grantlee modules directory to the CMake module path
set(CMAKE_MODULE_PATH ${Grantlee_PLUGIN_DIR}/cmake ${CMAKE_MODULE_PATH})

MACRO(PARSE_ARGUMENTS prefix arg_names option_names)
  SET(DEFAULT_ARGS)
  FOREACH(arg_name ${arg_names})
    SET(${prefix}_${arg_name})
  ENDFOREACH(arg_name)
  FOREACH(option ${option_names})
    SET(${prefix}_${option} FALSE)
  ENDFOREACH(option)

  SET(current_arg_name DEFAULT_ARGS)
  SET(current_arg_list)
  FOREACH(arg ${ARGN})
    SET(larg_names ${arg_names})
    LIST(FIND larg_names "${arg}" is_arg_name)
    IF (is_arg_name GREATER -1)
      SET(${prefix}_${current_arg_name} ${current_arg_list})
      SET(current_arg_name ${arg})
      SET(current_arg_list)
    ELSE (is_arg_name GREATER -1)
      SET(loption_names ${option_names})
      LIST(FIND loption_names "${arg}" is_option)
      IF (is_option GREATER -1)
             SET(${prefix}_${arg} TRUE)
      ELSE (is_option GREATER -1)
             SET(current_arg_list ${current_arg_list} ${arg})
      ENDIF (is_option GREATER -1)
    ENDIF (is_arg_name GREATER -1)
  ENDFOREACH(arg)
  SET(${prefix}_${current_arg_name} ${current_arg_list})
ENDMACRO(PARSE_ARGUMENTS)

macro(GRANTLEE_ADD_PLUGIN pluginname)
  parse_arguments(PLUGIN
      "TAGS;FILTERS"
      ""
      ${ARGN}
  )

  foreach(_filename ${PLUGIN_DEFAULT_ARGS})
    get_source_file_property(_skip ${_filename}.h SKIP_AUTOMOC)
    if (NOT _skip)
      set(_headers ${_headers} ${_filename}.h)
    endif()
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
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
  )
  target_link_libraries(${pluginname}
    ${Grantlee_CORE_LIBRARIES}
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
