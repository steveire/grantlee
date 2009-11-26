project(grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)
find_package(Automoc4 REQUIRED)

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


# Install FindGrantlee.cmake
install(FILES
    cmake/Modules/FindGrantlee.cmake
    DESTINATION ${DATA_INSTALL_DIR}/cmake/modules)
    
# Next target
add_subdirectory(grantlee_core_library)

add_subdirectory(grantlee_loadertags)
add_subdirectory(grantlee_defaulttags)

add_subdirectory(grantlee_defaultfilters)

add_subdirectory(grantlee_mutabletags)

add_subdirectory(grantlee_scriptabletags)

add_subdirectory(tests)

