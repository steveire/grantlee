project(grantlee_templates_tests)

#building on Solaris with Sun Studio uses stdcxx which does not have TR1 fully implemented
#take it from boost
if("${CMAKE_CXX_COMPILER_ID}" MATCHES "SunPro")
  find_package(Boost REQUIRED)
  include_directories(${Boost_INCLUDE_DIR} ${Boost_INCLUDE_DIR}/boost/tr1)
endif("${CMAKE_CXX_COMPILER_ID}" MATCHES "SunPro")

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( GRANTLEE_PLUGIN_PATH "${CMAKE_BINARY_DIR}/" )

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set(TESTS_LOCALES "fr_FR" "de_DE")

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

configure_file(testresource.qrc.cmake "${PROJECT_BINARY_DIR}/testresource.qrc")

set (_rcc_file "${PROJECT_BINARY_DIR}/testresource.qrc")
qt4_add_resources(_testresource_rcc_src ${_rcc_file} OPTIONS -root "/" )

macro(GRANTLEE_TEMPLATES_UNIT_TESTS)
  foreach(_testname ${ARGN})
    set(_testSrcs ${_testname}.cpp)
    set(moc_output_file "${CMAKE_CURRENT_BINARY_DIR}/${_testname}.moc")
    qt4_generate_moc(${_testname}.cpp ${moc_output_file})
    add_executable(${_testname}_exec
                  ${_testSrcs}
                  ${moc_output_file}
                  ${_testresource_rcc_src}
                  ${PROJECT_SOURCE_DIR}/../lib/lexer.cpp
                  ${PROJECT_SOURCE_DIR}/../lib/textprocessingmachine.cpp
                  ${PROJECT_SOURCE_DIR}/../lib/nulllocalizer.cpp )
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec ${QT_QTTEST_LIBRARY} grantlee_core )

    set_property(GLOBAL APPEND PROPERTY TEST_COVERAGE "${CMAKE_CURRENT_BINARY_DIR}/${_testname}_exec" )
  endforeach(_testname)
endmacro(GRANTLEE_TEMPLATES_UNIT_TESTS)

grantlee_templates_unit_tests(
  testsafestring
  testbuiltins
  testloadertags
  testdefaulttags
  testscriptabletags
  testfilters
  testgenerictypes
  testgenericcontainers
  testinternationalization
#   benchmarks
)

add_subdirectory(pluginpointertest)

