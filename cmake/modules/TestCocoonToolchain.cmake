# this one is important
SET(CMAKE_SYSTEM_NAME Linux)

find_program(TestCocoon_GCC csgcc)
find_program(TestCocoon_GXX csgxx)
find_program(TestCocoon_AR csar)

# specify the cross compiler
SET(CMAKE_C_COMPILER "${TestCocoon_GCC}"
    CACHE FILEPATH "CoverageScanner wrapper" FORCE)
SET(CMAKE_CXX_COMPILER "${TestCocoon_GXX}"
    CACHE FILEPATH "CoverageScanner wrapper" FORCE)
SET(CMAKE_LINKER "${TestCocoon_GXX}"
    CACHE FILEPATH "CoverageScanner wrapper" FORCE)
SET(CMAKE_AR "${TestCocoon_AR}"
    CACHE FILEPATH "CoverageScanner wrapper" FORCE)
