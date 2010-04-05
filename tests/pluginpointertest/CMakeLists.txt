cmake_minimum_required(VERSION 2.6)
project(sometest)

find_package(Qt4 REQUIRED)
include(${QT_USE_FILE})

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

include_directories(${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOOURCE_DIR}/grantlee_core )

set(myplugin_headers myobject.h)
set(myplugin_sources myobject.cpp)

qt4_wrap_cpp(_plugin_moc_srcs ${myplugin_headers})

add_library(myplugin SHARED ${myplugin_sources} ${_plugin_moc_srcs})
set_target_properties(myplugin
  PROPERTIES PREFIX ""
)
target_link_libraries(myplugin
  ${QT_QTCORE_LIBRARIES}
)

add_executable(main_app main.cpp ${myplugin_sources} ${_plugin_moc_srcs})
target_link_libraries(main_app
  ${QT_QTCORE_LIBRARIES}
)