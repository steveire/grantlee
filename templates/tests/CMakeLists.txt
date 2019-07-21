project(grantlee_templates_tests)

find_package(Qt5Test 5.2.0 REQUIRED)

#building on Solaris with Sun Studio uses stdcxx which does not have TR1 fully implemented
#take it from boost
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "SunPro")
  find_package(Boost REQUIRED)
  include_directories(${Boost_INCLUDE_DIR} ${Boost_INCLUDE_DIR}/boost/tr1)
endif("${CMAKE_CXX_COMPILER_ID}" MATCHES "SunPro")

include(CheckIncludeFileCXX)
check_include_file_cxx("tr1/unordered_map" HAVE_TR1_UNORDERED_MAP)
check_include_file_cxx("tr1/array" HAVE_TR1_ARRAY)
check_include_file_cxx("tr1/memory" HAVE_TR1_MEMORY)

check_cxx_source_compiles("

#include <tr1/unordered_map>

typedef std::tr1::unordered_map<int, int> std_unordered_mapintint;

int main(int, char **)
{
  std_unordered_mapintint m1;
  std_unordered_mapintint m2(m1);
  return 0;
}

" HAVE_WORKING_TR1_UNORDERED_MAP)

if(NOT HAVE_TR1_UNORDERED_MAP OR NOT HAVE_TR1_ARRAY OR NOT HAVE_TR1_MEMORY OR NOT HAVE_WORKING_TR1_UNORDERED_MAP)
  message(STATUS "Not enabling TR1 container tests")
  set(GRANTLEE_NO_TR1 True)
else()
  message(STATUS "Enabling TR1 container tests")
endif()

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( GRANTLEE_PLUGIN_PATH "${CMAKE_BINARY_DIR}/" )

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

find_package(Qt5LinguistTools QUIET)
if (Qt5LinguistTools_FOUND)
  set(TESTS_LOCALES "fr_FR" "de_DE")

  set(QT_LUPDATE_EXECUTABLE Qt5::lupdate)
  set(QT_LRELEASE_EXECUTABLE Qt5::lrelease)

  set(LINGUIST_TS_ARGS COMMAND "${QT_LUPDATE_EXECUTABLE}" "${CMAKE_CURRENT_SOURCE_DIR}/test_input.cpp" "-ts")

  foreach(TESTS_LOCALE ${TESTS_LOCALES})
      set( LINGUIST_TS_ARGS ${LINGUIST_TS_ARGS} "${CMAKE_CURRENT_SOURCE_DIR}/test_${TESTS_LOCALE}.ts" )
  endforeach()

  add_custom_target(test_l10n
    ${LINGUIST_TS_ARGS}
  )

  set( LINGUIST_QM_ARGS COMMAND "${QT_LRELEASE_EXECUTABLE}" )

  foreach(TESTS_LOCALE ${TESTS_LOCALES})
      set( generate_QMs ${generate_QMs} ${LINGUIST_QM_ARGS} "${CMAKE_CURRENT_SOURCE_DIR}/test_${TESTS_LOCALE}.ts" "-qm" "${CMAKE_CURRENT_BINARY_DIR}/test_${TESTS_LOCALE}.qm" )
  endforeach()

  add_custom_command(
    OUTPUT test_de_DE.qm test_fr_FR.qm
    ${generate_QMs}
    DEPENDS test_de_DE.ts test_fr_FR.ts
  )

  set(QM_FILES "
<qresource>
    <file alias=\"test_de_DE.qm\">${PROJECT_BINARY_DIR}/test_de_DE.qm</file>
    <file alias=\"test_fr_FR.qm\">${PROJECT_BINARY_DIR}/test_fr_FR.qm</file>
</qresource>")
endif()

configure_file(testresource.qrc.cmake "${PROJECT_BINARY_DIR}/testresource.qrc")

set (_rcc_file "${PROJECT_BINARY_DIR}/testresource.qrc")

qt5_add_resources(_testresource_rcc_src ${_rcc_file} OPTIONS -root "/" )

add_library(template_test_builtins STATIC coverageobject.cpp
  coverageobject.cpp
  ${_testresource_rcc_src}
)
target_link_libraries(template_test_builtins Qt5::Test)
target_compile_definitions(template_test_builtins PUBLIC cxx_auto_type)

macro(GRANTLEE_TEMPLATES_UNIT_TESTS)
  foreach(_testname ${ARGN})
    add_executable(${_testname}_exec
                  ${_testname}.cpp
    )
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec Grantlee5::Templates template_test_builtins)

    if (Qt5Qml_FOUND)
      target_compile_definitions(${_testname}_exec PRIVATE HAVE_QTQML_LIB)
    endif()

    set_property(GLOBAL APPEND PROPERTY TEST_COVERAGE "${CMAKE_CURRENT_BINARY_DIR}/${_testname}_exec" )
  endforeach(_testname)
endmacro(GRANTLEE_TEMPLATES_UNIT_TESTS)

grantlee_templates_unit_tests(
  testsafestring
  testbuiltins
  testloadertags
  testdefaulttags
  testcachingloader
  testfilters
  testgenerictypes
  testgenericcontainers
#   benchmarks
)

if (Qt5Qml_FOUND)
  grantlee_templates_unit_tests(
    testscriptabletags
  )
endif()

if (Qt5LinguistTools_FOUND)
  grantlee_templates_unit_tests(
    testinternationalization
  )
  if ("x${CMAKE_CXX_COMPILER_ID}x" MATCHES "xMSVCx")
    target_compile_options(testinternationalization_exec PRIVATE "/utf-8")
  endif()
endif()
