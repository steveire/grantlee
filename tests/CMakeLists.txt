project(grantleetests)

enable_testing()
include(${QT_USE_FILE})

include_directories( ${QT_INCLUDES}
  ${CMAKE_CURRENT_BINARY_DIR}
  ${PROJECT_SOURCE_DIR}/../grantlee_core_library
)

set( EXECUTABLE_OUTPUT_PATH ${CMAKE_CURRENT_BINARY_DIR} )

set( GRANTLEE_PLUGIN_PATH "${LIB_INSTALL_DIR}/grantlee/" )

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

set (_rcc_file "scriptedtags.qrc")
qt4_add_resources(_scripted_rcc_src ${_rcc_file} OPTIONS -root "/grantlee/0.1" )

macro(GRANTLEE_UNIT_TESTS)
  foreach(_testname ${ARGN})
    set(_testSrcs ${_testname}.cpp)
    set(moc_output_file "${CMAKE_CURRENT_BINARY_DIR}/${_testname}.moc")
    qt4_generate_moc(${_testname}.cpp ${moc_output_file})
    add_executable(${_testname}_exec ${_testSrcs} ${moc_output_file} ${_scripted_rcc_src})
    add_test(${_testname} ${_testname}_exec )
    target_link_libraries(${_testname}_exec ${QT_QTTEST_LIBRARY} ${QT_QTGUI_LIBRARY} grantlee_core_library)
  endforeach(_testname)
endmacro(GRANTLEE_UNIT_TESTS)

grantlee_unit_tests(
  testbuiltins
  testloadertags
  testdefaulttags
  testscriptabletags
  testmutabletags
#   benchmarks
  testfilters
)

