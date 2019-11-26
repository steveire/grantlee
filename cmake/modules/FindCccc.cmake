# - Find cccc
#
# CCCC_EXECUTABLE - the cccc binary


include(FindPackageHandleStandardArgs)

find_program(CCCC_EXECUTABLE cccc)

find_package_handle_standard_args(Cccc DEFAULT_MSG CCCC_EXECUTABLE)

mark_as_advanced(CCCC_EXECUTABLE)
