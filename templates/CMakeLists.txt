
find_package(Qt5Core 5.2.0 REQUIRED)
find_package(Qt5Qml 5.2.0)

if (TARGET Qt5::Qml)
    set_property(TARGET Qt5::Qml APPEND PROPERTY INTERFACE_COMPILE_DEFINITIONS QT_SCRIPT_LIB)
endif()

add_subdirectory(lib)

if (BUILD_MAIN_PLUGINS)
  add_subdirectory(loadertags)
  add_subdirectory(defaulttags)

  add_subdirectory(defaultfilters)
endif()


if (BUILD_I18N_PLUGIN)
  add_subdirectory(i18n)
endif()

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()
