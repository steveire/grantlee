project(grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)
find_package(Automoc4 REQUIRED)

set(GRANTLEE_LIB_MAJOR_VERSION 0)
set(GRANTLEE_LIB_MINOR_VERSION 0)
set(GRANTLEE_LIB_PATCH_VERSION 0)

set( LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib )
set( BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin )
set( INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include )

add_subdirectory(grantlee_core_library)

add_subdirectory(grantlee_loadertags)
add_subdirectory(grantlee_defaulttags)

# add_subdirectory(grantlee_scriptabletags)

add_subdirectory(tests)
