project(Grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 REQUIRED)

set(CMAKE_MODULE_PATH ${Grantlee_SOURCE_DIR}/cmake/modules)

if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -ansi -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-check-new -fno-common -pedantic -fvisibility=hidden")
endif()

set(Grantlee_VERSION_MAJOR 0)
set(Grantlee_VERSION_MINOR 1)
set(Grantlee_VERSION_PATCH 0)
set(Grantlee_MIN_PLUGIN_VERSION 1)

set(Grantlee_MAJOR_MINOR_VERSION_STRING "${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}" )
set(Grantlee_VERSION_STRING "${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}" )

set (LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (eg. '64')")

set( LIB_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX} )
set( PLUGIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib${LIB_SUFFIX}/grantlee/${Grantlee_MAJOR_MINOR_VERSION_STRING} )
set( BIN_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/bin )
set( INCLUDE_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/include )
set( DATA_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/share/apps )
set( CMAKE_INSTALL_NAME_DIR ${LIB_INSTALL_DIR})

set(Grantlee_USE_FILE "${Grantlee_SOURCE_DIR}/cmake/modules/GrantleeUse.cmake")

add_subdirectory(grantlee_core_library)

add_subdirectory(grantlee_loadertags)
add_subdirectory(grantlee_defaulttags)

add_subdirectory(grantlee_defaultfilters)

add_subdirectory(grantlee_mutabletags)

add_subdirectory(grantlee_scriptabletags)

add_subdirectory(tests)

configure_file(
  "${CMAKE_MODULE_PATH}/GrantleeConfig.cmake.in"
  "${Grantlee_BINARY_DIR}/GrantleeConfig.cmake"
  IMMEDIATE @ONLY
)

configure_file(
  "${CMAKE_MODULE_PATH}/GrantleeConfigVersion.cmake.in"
  "${Grantlee_BINARY_DIR}/GrantleeConfigVersion.cmake"
  IMMEDIATE @ONLY
)

install(FILES
  "${Grantlee_BINARY_DIR}/GrantleeConfig.cmake"
  "${Grantlee_BINARY_DIR}/GrantleeConfigVersion.cmake"
  DESTINATION "${LIB_INSTALL_DIR}/grantlee")

