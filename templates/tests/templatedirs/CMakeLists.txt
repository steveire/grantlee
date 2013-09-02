project(template_dirs_test)

cmake_minimum_required(VERSION 2.8.11)

find_package(Grantlee REQUIRED)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

configure_file(grantlee_paths.h.cmake ${PROJECT_BINARY_DIR}/grantlee_paths.h)

execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/template_dir_1 ${PROJECT_BINARY_DIR}/template_dir_1)
execute_process(COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/template_dir_2 ${PROJECT_BINARY_DIR}/template_dir_2)

set(template_dirs_test_SRCS
  main.cpp
)

add_executable(template_dirs_test
  ${template_dirs_test_SRCS}
)

target_link_libraries(
  template_dirs_test
  Qt4::QtCore
  ${Grantlee_TEMPLATES_LIBRARIES}
)


