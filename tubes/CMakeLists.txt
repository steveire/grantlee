
find_package(Qt4 4.5.0 REQUIRED QtCore)

add_subdirectory(lib)

include_directories(
  lib
)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()

