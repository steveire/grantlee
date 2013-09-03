
# Set up the include directories and link directories
include_directories(${Grantlee_INCLUDE_DIRS})

# Add the Grantlee modules directory to the CMake module path
set(CMAKE_MODULE_PATH ${Grantlee_MODULE_DIR} ${CMAKE_MODULE_PATH})

include(CMakeParseArguments)

macro(grantlee_adjust_plugin_name pluginname)
  set_target_properties(${pluginname}
    PROPERTIES PREFIX ""
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/grantlee/${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}"
  )
endmacro()
