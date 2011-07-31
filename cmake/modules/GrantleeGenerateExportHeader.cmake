
# - Macro for generation of export macros for libraries
#
# This module provides the macro GENERATE_EXPORT_HEADER().
#
# This macro can be used to generate a file suitable for preprocessor
# inclusion which contains EXPORT macros to be used in library classes.
#
# GENERATE_EXPORT_HEADER( LIBRARY_TARGET
#                         [BASE_NAME <base_name>]
#                         [EXPORT_MACRO_NAME <export_macro_name>]
#                         [EXPORT_FILE_NAME <export_file_name>]
#                         [DEPRECATED_NAME <deprecated_name>]
#                         [NO_EXPORT_MACRO_NAME <no_export_macro_name>]
#                         [STATIC_DEFINE <static_define>]
# )
#
# By default GENERATE_EXPORT_HEADER() generates macro names in a file name
# determined by the name of the library. For example,
#
#     add_library(somelib someclass.cpp)
#     generate_export_header(somelib)
#
# will generate a file in the ${CMAKE_CURRENT_BUILD_DIR} called somelib_export.h
# containing the macros SOMELIB_EXPORT, SOMELIB_NO_EXPORT and
# SOMELIB_DEPRECATED. The resulting file should be installed with other headers
# in the library.
#
# The BASE_NAME argument can be used to override the file name and the names
# used for the macros
#
#     add_library(somelib someclass.cpp)
#     generate_export_header(somelib
#         BASE_NAME other_name
#     )
#
# Generates a file called other_name_export.h containing the macros
# OTHER_NAME_EXPORT, OTHER_NAME_NO_EXPORT and OTHER_NAME_DEPRECATED.
#
# The BASE_NAME may be overridden by specifiying other options in the macro.
# For example:
#
#     add_library(somelib someclass.cpp)
#     generate_export_header(somelib
#         EXPORT_MACRO_NAME OTHER_NAME_EXPORT
#     )
#
# creates the macro OTHER_NAME_EXPORT instead of SOMELIB_EXPORT.
#
#     add_library(somelib someclass.cpp)
#     generate_export_header(somelib
#         DEPRECATED_NAME KDE_DEPRECATED
#     )
#
# creates the macro KDE_DEPRECATED instead of SOMELIB_DEPRECATED.
#
# If LIBRARY_TARGET is a static library, macros are defined without values.
#
# If the same sources are used to create both a shared and a static library, the
# uppercased symbol ${BASE_NAME}_STATIC_DEFINE should be used when building the
# static library
#
#   add_library(shared_variant SHARED ${lib_SRCS})
#   add_library(static_variant ${lib_SRCS})
#
#   generate_export_header(shared_variant BASE_NAME libshared_and_static)
#
#   set_target_properties(static_variant PROPERTIES DEFINE_SYMBOL LIBSHARED_AND_STATIC_STATIC_DEFINE)
#

include(CMakeParseArguments)
include(CheckCXXCompilerFlag)

# Workaround for http://public.kitware.com/Bug/view.php?id=12265
get_filename_component(myDir ${CMAKE_CURRENT_LIST_FILE} PATH)

macro(_DO_SET_MACRO_VALUES TARGET_LIBRARY)
    set(DEFINE_DEPRECATED)
    set(DEFINE_EXPORT)
    set(DEFINE_IMPORT)
    set(DEFINE_NO_EXPORT)

    if (MSVC OR CYGWIN OR MINGW)
        set(DEFINE_DEPRECATED "__declspec(deprecated)")
    else()
        set(DEFINE_DEPRECATED "__attribute__ ((__deprecated__))")
    endif()

    get_property(type TARGET ${TARGET_LIBRARY} PROPERTY TYPE)

    if (NOT ${type} STREQUAL "STATIC_LIBRARY")
        if (MSVC OR CYGWIN OR MINGW)
            set(DEFINE_EXPORT "__declspec(dllexport)")
            set(DEFINE_IMPORT "__declspec(dllimport)")
        else()
            check_cxx_compiler_flag(-fvisibility=hidden HAVE_GCC_VISIBILITY)
            set(HAVE_GCC_VISIBILITY ${HAVE_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")
            if (${HAVE_GCC_VISIBILITY})
                set(DEFINE_EXPORT "__attribute__((visibility(\"default\")))")
                set(DEFINE_IMPORT "__attribute__((visibility(\"default\")))")
                set(DEFINE_NO_EXPORT "__attribute__((visibility(\"hidden\")))")
            endif()
        endif()
    endif()
endmacro()

macro(_DO_GENERATE_EXPORT_HEADER TARGET_LIBRARY)
    # Option overrides
    set(options)
    set(oneValueArgs BASE_NAME EXPORT_MACRO_NAME EXPORT_FILE_NAME DEPRECATED_NAME NO_EXPORT_MACRO_NAME STATIC_DEFINE)
    set(multiValueArgs)

    cmake_parse_arguments(_GEH "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

    set(BASE_NAME "${TARGET_LIBRARY}")

    if (_GEH_BASE_NAME)
        set(BASE_NAME ${_GEH_BASE_NAME})
    endif()

    string(TOUPPER ${BASE_NAME} BASE_NAME_UPPER)
    string(TOLOWER ${BASE_NAME} BASE_NAME_LOWER)

    # Default options
    set(EXPORT_MACRO_NAME "${BASE_NAME_UPPER}_EXPORT")
    set(NO_EXPORT_MACRO_NAME "${BASE_NAME_UPPER}_NO_EXPORT")
    set(EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${BASE_NAME_LOWER}_export.h")
    set(DEPRECATED_NAME "${BASE_NAME_UPPER}_DEPRECATED")
    set(STATIC_DEFINE "${BASE_NAME_UPPER}_STATIC_DEFINE")

    if(_GEH_UNPARSED_ARGUMENTS)
        message(FATAL_ERROR "Unknown keywords given to GENERATE_EXPORT_HEADER(): \"${_GEH_UNPARSED_ARGUMENTS}\"")
    endif()

    if (_GEH_EXPORT_MACRO_NAME)
        set(EXPORT_MACRO_NAME ${_GEH_EXPORT_MACRO_NAME})
    endif()
    if (_GEH_EXPORT_FILE_NAME)
        if (IS_ABSOLUTE _GEH_EXPORT_FILE_NAME)
            set(EXPORT_FILE_NAME ${_GEH_EXPORT_FILE_NAME})
        else()
            set(EXPORT_FILE_NAME "${CMAKE_CURRENT_BINARY_DIR}/${_GEH_EXPORT_FILE_NAME}")
        endif()
    endif()
    if (_GEH_DEPRECATED_NAME)
        set(DEPRECATED_NAME ${_GEH_DEPRECATED_NAME})
    endif()
    if (_GEH_NO_EXPORT_MACRO_NAME)
        set(NO_EXPORT_MACRO_NAME ${_GEH_NO_EXPORT_MACRO_NAME})
    endif()
    if (_GEH_STATIC_DEFINE)
        set(STATIC_DEFINE ${_GEH_STATIC_DEFINE})
    endif()

    set(INCLUDE_GUARD_NAME "${EXPORT_MACRO_NAME}_H")
    set(EXPORT_IMPORT_CONDITION "${BASE_NAME_UPPER}_MAKEDLL")

    configure_file(${myDir}/exportheader.cmake.in ${EXPORT_FILE_NAME} @ONLY)

    set_target_properties(${TARGET_LIBRARY} PROPERTIES DEFINE_SYMBOL ${EXPORT_IMPORT_CONDITION})
endmacro()

function(GRANTLEE_GENERATE_EXPORT_HEADER TARGET_LIBRARY)
    get_property(type TARGET ${TARGET_LIBRARY} PROPERTY TYPE)
    if (NOT ${type} STREQUAL "STATIC_LIBRARY" AND NOT ${type} STREQUAL "SHARED_LIBRARY")
        message(WARNING "This macro can only be used with libraries")
        return()
    endif()
    _do_set_macro_values(${TARGET_LIBRARY})
    _do_generate_export_header(${TARGET_LIBRARY} ${ARGN})
endfunction()

function(grantlee_enable_gcc_hidden_visibility)

  set(options)
  set(oneValueArgs FATAL_WARNINGS)
  set(multiValueArgs)

  cmake_parse_arguments(_EGHV "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  set(MESSAGE_TYPE WARNING)

  if (_EGHV_FATAL_WARNINGS)
    set(MESSAGE_TYPE FATAL_ERROR)
  endif()

  if (NOT CMAKE_COMPILER_IS_GNUCXX)
    message(${MESSAGE_TYPE} "Thid feature is only available with GCC")
    return()
  endif()

  exec_program(${CMAKE_C_COMPILER} ARGS --version OUTPUT_VARIABLE       _gcc_version_info)
  string (REGEX MATCH "[345]\\.[0-9]\\.[0-9]" _gcc_version "${_gcc_version_info}")
  # gcc on mac just reports: "gcc (GCC) 3.3 20030304 ..." without the
  # patch level, handle this here:
  if (NOT _gcc_version)
    string (REGEX REPLACE ".*\\(GCC\\).* ([34]\\.[0-9]) .*" "\\1.0" _gcc_version "${_gcc_version_info}")
  endif ()

  check_cxx_compiler_flag(-fvisibility=hidden HAVE_GCC_VISIBILITY)
  check_cxx_compiler_flag(-fvisibility-inlines-hidden HAVE_GCC_INLINE_VISIBILITY)
  set(HAVE_GCC_VISIBILITY ${HAVE_GCC_VISIBILITY} CACHE BOOL "GCC support for hidden visibility")

  set(_GCC_COMPILED_WITH_BAD_ALLOCATOR FALSE)
  if (HAVE_GCC_VISIBILITY)
    exec_program(${CMAKE_C_COMPILER} ARGS -v OUTPUT_VARIABLE _gcc_alloc_info)
    string(REGEX MATCH "(--enable-libstdcxx-allocator=mt)" _GCC_COMPILED_WITH_BAD_ALLOCATOR "${_gcc_alloc_info}")
  endif ()

  if (HAVE_GCC_VISIBILITY AND NOT _GCC_COMPILED_WITH_BAD_ALLOCATOR)
    # We have all the parts necessary - use visibility support
    add_definitions(-DCMAKE_HAVE_GCC_VISIBILITY)

    set (EXTRA_FLAGS "-fvisibility=hidden")

    if (HAVE_GCC_INLINE_VISIBILITY)
      set (EXTRA_FLAGS "${EXTRA_FLAGS} -fvisibility-inlines-hidden")
    else()
      message(${MESSAGE_TYPE} "GCC Hidden visibility of inlines not available for this compiler")
    endif ()
  else()
    message(${MESSAGE_TYPE} "GCC Hidden visibility not available for this compiler")
  endif ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${EXTRA_FLAGS}" PARENT_SCOPE)
endfunction()

