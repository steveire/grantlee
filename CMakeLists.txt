project(grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)

if (UNIX)
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -ansi -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-check-new -fno-common -pedantic")
endif()

set(GRANTLEE_VERSION_MAJOR 0)
set(GRANTLEE_VERSION_MINOR 1)
set(GRANTLEE_VERSION_PATCH 0)

set(GRANTLEE_MAJOR_MINOR_VERSION_STRING "${GRANTLEE_VERSION_MAJOR}.${GRANTLEE_VERSION_MINOR}" )
set(GRANTLEE_VERSION_STRING "${GRANTLEE_VERSION_MAJOR}.${GRANTLEE_VERSION_MINOR}.${GRANTLEE_VERSION_PATCH}" )

set (LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (eg. '64')")

set( LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} )
set( PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/grantlee/${GRANTLEE_MAJOR_MINOR_VERSION_STRING} )
set( BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin )
set( INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include )
set( DATA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/apps )
set( CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

macro(GRANTLEE_ADD_PLUGIN pluginname )
  foreach(_filename ${ARGN})
    set(_headers ${_headers} ${_filename}.h)
    set(_sources ${_sources} ${_filename}.cpp)
  endforeach(_filename ${ARGN})
  qt4_wrap_cpp(_plugin_moc_srcs ${_headers})
  add_library(${pluginname} SHARED ${_sources} ${_plugin_moc_srcs})
  set_target_properties(${pluginname} PROPERTIES PREFIX "")
endmacro(GRANTLEE_ADD_PLUGIN)


# Install FindGrantlee.cmake
install(FILES
    cmake/Modules/FindGrantlee.cmake
    DESTINATION ${DATA_INSTALL_DIR}/cmake/modules)
    
add_subdirectory(grantlee_core_library)

add_subdirectory(grantlee_loadertags)
add_subdirectory(grantlee_defaulttags)

add_subdirectory(grantlee_defaultfilters)

add_subdirectory(grantlee_mutabletags)

add_subdirectory(grantlee_scriptabletags)

add_subdirectory(tests)

