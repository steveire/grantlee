
include(CMakeParseArguments)

macro(grantlee_adjust_plugin_name pluginname)
  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee5_MAJOR_MINOR_VERSION_STRING}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee5_MAJOR_MINOR_VERSION_STRING}"
    DEBUG_POSTFIX "d"
  )
  foreach(cfg ${CMAKE_CONFIGURATION_TYPES})
    string(TOUPPER ${cfg} CFG)
    set_target_properties(${pluginname}
      PROPERTIES
      LIBRARY_OUTPUT_DIRECTORY_${CFG} "${CMAKE_BINARY_DIR}/grantlee/${Grantlee5_MAJOR_MINOR_VERSION_STRING}"
      RUNTIME_OUTPUT_DIRECTORY_${CFG} "${CMAKE_BINARY_DIR}/grantlee/${Grantlee5_MAJOR_MINOR_VERSION_STRING}"
      )
  endforeach()
endmacro()
