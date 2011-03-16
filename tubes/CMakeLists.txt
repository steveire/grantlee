
find_package(Qt4 4.5.0 REQUIRED QtCore)

include(${QT_USE_FILE})

add_subdirectory(lib)

include_directories(
  lib
)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()

