# - Find lcov
#
# LCOV_EXECUTABLE - the lcov binary
# GENHTML_EXECUTABLE - the genhtml executable
#

include(FindPackageHandleStandardArgs)

find_program(LCOV_EXECUTABLE lcov)
find_program(GENHTML_EXECUTABLE genhtml)

find_package_handle_standard_args(LCov DEFAULT_MSG LCOV_EXECUTABLE GENHTML_EXECUTABLE)

mark_as_advanced(LCOV_EXECUTABLE GENHTML_EXECUTABLE)
