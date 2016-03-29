
include(CMakeParseArguments)

macro(grantlee_adjust_plugin_name pluginname)
  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee5_VERSION_MAJOR}.${Grantlee5_VERSION_MINOR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee5_VERSION_MAJOR}.${Grantlee5_VERSION_MINOR}"
    DEBUG_POSTFIX "d"
  )
endmacro()
