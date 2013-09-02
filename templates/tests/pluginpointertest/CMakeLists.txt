cmake_minimum_required(VERSION 2.8.11)
project(pluginpointertest)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set(myplugin_headers myobject.h)
set(myplugin_sources myobject.cpp)

add_library(myplugin SHARED ${myplugin_sources} ${_plugin_moc_srcs})
set_target_properties(myplugin
  PROPERTIES PREFIX ""
)
target_link_libraries(myplugin
  Qt4::QtCore
)

add_executable(main_app main.cpp ${myplugin_sources} ${_plugin_moc_srcs})
target_include_directories(main_app
  PRIVATE
    $<TARGET_PROPERTY:grantlee_core,INTERFACE_INCLUDE_DIRECTORIES>
)
target_link_libraries(main_app
  Qt4::QtCore
)
