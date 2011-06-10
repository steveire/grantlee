project(Grantlee)

cmake_minimum_required(VERSION 2.6.4)

option( BUILD_CORE "Build the Grantlee template library. Deprecated. Use BUILD_TEMPLATES instead" TRUE )
option( BUILD_GUI "Build the Grantlee textdocument library. Deprecated. Use BUILD_TEXTDOCUMENT instead" TRUE )
option( BUILD_TEMPLATES "Build the Grantlee template library" TRUE )
option( BUILD_TEXTDOCUMENT "Build the Grantlee textdocument library" TRUE )
option( BUILD_MAIN_PLUGINS "Build the Grantlee Templates plugins" TRUE )
option( BUILD_I18N_PLUGIN "Build the Grantlee Templates i18n plugin" TRUE )
option( BUILD_SCRIPT_PLUGIN "Build the Grantlee Templates scriptable tags plugin" TRUE )
option( BUILD_TESTS "Build the Grantlee tests" TRUE )

if (BUILD_TESTS)
  enable_testing()
endif()

set(Grantlee_MODULE_DIR ${Grantlee_SOURCE_DIR}/cmake/modules)
set(CMAKE_MODULE_PATH ${Grantlee_MODULE_DIR})

include(GrantleeGenerateExportHeader)

if (CMAKE_COMPILER_IS_GNUCXX)
  set (CMAKE_C_FLAGS   "${CMAKE_C_FLAGS} -Wno-long-long -std=iso9899:1990 -Wundef -Wcast-align -Werror-implicit-function-declaration -Wchar-subscripts -Wall -W -Wpointer-arith -Wwrite-strings -Wformat-security -Wmissing-format-attribute -fno-common")
  set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wnon-virtual-dtor -Wno-long-long -ansi -Wundef -Wcast-align -Wchar-subscripts -Wall -W -Wpointer-arith -Wformat-security -fno-check-new -fno-common -pedantic-errors")
  if (NOT MINGW)
    enable_gcc_hidden_visibility()
  else ()
    # http://mail.kde.org/pipermail/kde-windows/2007-December/001692.html
    # http://lists.trolltech.com/pipermail/qt-interest/2009-July/009829.html
    # qt is always compiled with QT_NO_DEBUG under mingw,
    # so we need to compile stuff linked against it
    # the same or otherwise QPluginLoader rejects plugins
    add_definitions(-DQT_NO_DEBUG)
  endif ()
endif()

add_definitions(-DQT_NO_CAST_TO_ASCII)
add_definitions(-DQT_NO_CAST_FROM_ASCII)
add_definitions(-DQT_STRICT_ITERATORS)
add_definitions(-DQT_NO_URL_CAST_FROM_STRING)
add_definitions(-DQT_NO_CAST_FROM_BYTEARRAY)

set(Grantlee_VERSION_MAJOR 0)
set(Grantlee_VERSION_MINOR 1)
set(Grantlee_VERSION_PATCH 9)
set(Grantlee_MIN_PLUGIN_VERSION 1)

set(Grantlee_MAJOR_MINOR_VERSION_STRING "${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}" )
set(Grantlee_VERSION_STRING "${Grantlee_VERSION_MAJOR}.${Grantlee_VERSION_MINOR}.${Grantlee_VERSION_PATCH}" )

set (LIB_SUFFIX "" CACHE STRING "Define suffix of library directory name (eg. '64')")

set( LIB_INSTALL_DIR lib${LIB_SUFFIX} )
set( PLUGIN_INSTALL_DIR lib${LIB_SUFFIX}/grantlee/${Grantlee_MAJOR_MINOR_VERSION_STRING} )
set( BIN_INSTALL_DIR bin )
set( INCLUDE_INSTALL_DIR include )
set( DATA_INSTALL_DIR share/apps )

# set up RPATH/install_name_dir
set( CMAKE_INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/${LIB_INSTALL_DIR})
set( CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)

set(Grantlee_USE_FILE "${Grantlee_MODULE_DIR}/GrantleeUse.cmake")

if (BUILD_CORE AND BUILD_TEMPLATES)
  add_subdirectory(templates)
endif()

if (BUILD_GUI AND BUILD_TEXTDOCUMENT)
  add_subdirectory(textdocument)
endif()

set(CMAKECONFIG_INSTALL_DIR "${LIB_INSTALL_DIR}/cmake/grantlee" )

file(RELATIVE_PATH relInstallDir ${CMAKE_INSTALL_PREFIX}/${CMAKECONFIG_INSTALL_DIR} ${CMAKE_INSTALL_PREFIX} )

configure_file(
  "${Grantlee_MODULE_DIR}/GrantleeConfig.cmake.in"
  "${Grantlee_BINARY_DIR}/GrantleeConfig.cmake"
  @ONLY
)

configure_file(
  "${Grantlee_MODULE_DIR}/GrantleeConfigVersion.cmake.in"
  "${Grantlee_BINARY_DIR}/GrantleeConfigVersion.cmake"
  @ONLY
)

install(FILES
  "${Grantlee_BINARY_DIR}/GrantleeConfig.cmake"
  "${Grantlee_BINARY_DIR}/GrantleeConfigVersion.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}")

install(FILES
  "${Grantlee_MODULE_DIR}/GrantleeUse.cmake"
  DESTINATION "${CMAKECONFIG_INSTALL_DIR}"
)

install(EXPORT grantlee_targets DESTINATION "${CMAKECONFIG_INSTALL_DIR}" FILE GrantleeTargets.cmake )

# Add support for an uninstall target
configure_file(
  "${Grantlee_MODULE_DIR}/cmake_uninstall.cmake.in"
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake"
  IMMEDIATE @ONLY)

add_custom_target(uninstall
  "${CMAKE_COMMAND}" -P
  "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

find_package(Doxygen)
if (DOXYGEN_FOUND AND DOXYGEN_DOT_FOUND)
  message (STATUS "Doxygen and dot found. make docs target is available")
  # Add support for a docs target

  # Available from http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen/libstdc++.tag
  set(STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++.tag=http://gcc.gnu.org/onlinedocs/libstdc++/latest-doxygen")
  set(QTTAGFILE "${CMAKE_SOURCE_DIR}/scripts/qttags.tag=http://qt.nokia.com/doc/${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}")

  set(TAGFILES "${QTTAGFILE} ${STLTAGFILE}")
  set(DOCS_OUTPUT_DIRECTORY "@CMAKE_BINARY_DIR@")
  configure_file(
    "${Grantlee_MODULE_DIR}/Doxyfile.in"
    "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile"
    @ONLY)

  # Add support for a Qt assistant docs target
  # qttags.tag is too big to distribute with Grantlee. Get it
  # from http://api.kde.org/kdereview-api/kdereview.tar.gz for example.
  if(EXISTS ${CMAKE_SOURCE_DIR}/scripts/qttags.tag)
    set(DOCS_OUTPUT_DIRECTORY "@CMAKE_BINARY_DIR@/qtdocs")

    # Available from http://steveire.com/libstdc++-longnames.tag
    # Also recommended: http://www.steveire.com/libstdc++-v3.qch
    set(STLTAGFILE "${CMAKE_SOURCE_DIR}/scripts/libstdc++-longnames.tag=qthelp://org.gnu.libstdcxx/libstdc++-v3/")
    set(QTTAGFILE "${CMAKE_SOURCE_DIR}/scripts/qttags.tag=qthelp://com.trolltech.qt/qdoc/")

    set(TAGFILES "${QTTAGFILE} ${STLTAGFILE}")
    set(GENERATE_QHP "yes")

    configure_file(
      "${Grantlee_MODULE_DIR}/Doxyfile.in"
      "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt"
      @ONLY)
  endif()

  set( DOCS_ARGS ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile" )
  if(EXISTS "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt")
    set(DOCS_ARGS ${DOCS_ARGS} COMMAND ${DOXYGEN_EXECUTABLE} "${CMAKE_CURRENT_BINARY_DIR}/Doxyfile.qt" )
  endif()

  add_custom_target(docs
    ${DOCS_ARGS}
  )
endif()

