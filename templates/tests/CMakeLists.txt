project(grantlee_templates_tests)

include_directories( ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( GRANTLEE_PLUGIN_PATH "${CMAKE_BINARY_DIR}/" )

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set (_rcc_file "scriptedtags.qrc")
qt4_add_resources(_scripted_rcc_src ${_rcc_file} OPTIONS -root "/plugins/grantlee/${Grantlee_MAJOR_MINOR_VERSION_STRING}" )

macro(GRANTLEE_CORE_UNIT_TESTS)
  foreach(_testname ${ARGN})
    set(_testSrcs ${_testname}.cpp)
    set(moc_output_file "${CMAKE_CURRENT_BINARY_DIR}/${_testname}.moc")
    qt4_generate_moc(${_testname}.cpp ${moc_output_file})
    add_executable(${_testname}_exec ${_testSrcs} ${moc_output_file} ${_scripted_rcc_src} ${PROJECT_SOURCE_DIR}/../lib/lexer.cpp)
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec ${QT_QTTEST_LIBRARY} ${Grantlee_CORE_LIBRARIES} )
  endforeach(_testname)
endmacro(GRANTLEE_CORE_UNIT_TESTS)

grantlee_core_unit_tests(
  testsafestring
  testbuiltins
  testloadertags
  testdefaulttags
  testscriptabletags
  testfilters
  testgenerictypes
  testgenericcontainers
  benchmarks
)

add_subdirectory(pluginpointertest)

