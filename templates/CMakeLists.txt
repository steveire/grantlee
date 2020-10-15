
if (GRANTLEE_BUILD_WITH_QT6)
    find_package(Qt6Core 6.0.0 REQUIRED)
    find_package(Qt6Qml 6.0.0)
else()
    find_package(Qt5Core 5.2.0 REQUIRED)
    find_package(Qt5Qml 5.2.0)
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
