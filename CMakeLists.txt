#SET ( CMAKE_BUILD_TYPE "Debug" )
SET ( CMAKE_BUILD_TYPE "Release" )

project(HACKTM)

add_definitions("-Wall -D__STDC_LIMIT_MACROS -D__STDC_CONSTANT_MACROS")


#include(FindOpenMP)
#if(OpenMP_FLAG_DETECTED)
#   add_definitions(${OpenMP_CXX_FLAGS})
#endif(OpenMP_FLAG_DETECTED)

include_directories(${HACKTM_SOURCE_DIR}/)
include_directories(${HACKTM_SOURCE_DIR}/hacktm)
include_directories(${HACKTM_BINARY_DIR}/include)
include_directories(${HACKTM_SOURCE_DIR}/include)

link_directories(${HACKTM_SOURCE_DIR}/hacktm)

add_subdirectory(hacktm)
add_subdirectory(tests)
add_subdirectory(include)

