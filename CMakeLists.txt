SET ( CMAKE_BUILD_TYPE "Debug" )
project(HACKTM)

include_directories(${HACKTM_SOURCE_DIR}/HackTM)
include_directories(${HACKTM_SOURCE_DIR}/)
include_directories(${HACKTM_SOURCE_DIR}/include)

link_directories(${HACKTM_SOURCE_DIR}/HackTM)
link_directories(${HACKTM_SOURCE_DIR}/metric)

add_subdirectory(HackTM)
add_subdirectory(metric)
add_subdirectory(tests)

