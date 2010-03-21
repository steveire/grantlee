project(Grantlee)

cmake_minimum_required(VERSION 2.6)

find_package(Qt4 4.5.0 REQUIRED)

option( BUILD_CORE "Build the Grantlee core library" TRUE )
option( BUILD_GUI "Build the Grantlee gui library" TRUE )
option( BUILD_PLUGINS "Build the Grantlee plugins" TRUE )
option( BUILD_TESTS "Build the Grantlee tests" FALSE )

set(CMAKE_MODULE_PATH ${Grantlee_SOURCE_DIR}/cmake/modules)

if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -ansi -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-check-new -fno-common -pedantic")
  if (NOT MINGW)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fvisibility=hidden")
  else ()
    # http://mail.kde.org/pipermail/kde-windows/2007-December/001692.html
    # http://lists.trolltech.com/pipermail/qt-interest/2009-July/009829.html
    # qt is always compiled with QT_NO_DEBUG under mingw,
    # so we need to compile stuff linked against it
    # the same or otherwise QPluginLoader rejects plugins
    add_definitions(-DQT_NO_DEBUG)
  endif ()
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

if (BUILD_CORE)
  set(Grantlee_CORE_LIBRARY "grantlee_core")
  set(Grantlee_CORE_LIBRARIES ${Grantlee_CORE_LIBRARY} ${QT_QTCORE_LIBRARIES} )
  add_subdirectory(corelib)
endif()

if (BUILD_GUI)
  set(Grantlee_GUI_LIBRARY "grantlee_gui")
  set(Grantlee_GUI_LIBRARIES ${Grantlee_GUI_LIBRARY} ${QT_QTGUI_LIBRARIES} )
  add_subdirectory(gui)
endif()

if (BUILD_PLUGINS)
  add_subdirectory(loadertags)
  add_subdirectory(defaulttags)

  add_subdirectory(defaultfilters)

  add_subdirectory(mutabletags)

  add_subdirectory(scriptabletags)
endif()

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()

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

install(FILES
  "${CMAKE_MODULE_PATH}/GrantleeUse.cmake"
  DESTINATION "${PLUGIN_INSTALL_DIR}"
)

# Add support for an uninstall target
configure_file(
  "${CMAKE_MODULE_PATH}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

# Add support for a docs target
configure_file(
  "${CMAKE_MODULE_PATH}/Doxyfile.in"
  "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  IMMEDIATE @ONLY)

find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
  message (STATUS "Doxygen and dot found. make docs target is available")
  add_custom_target(docs
    ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
  )
endif()

