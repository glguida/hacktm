add_executable(testRegion testRegion.cpp)
add_executable(testMM testMM.cpp)
add_executable(testNode testNode.cpp)
add_executable(benchNode benchNode.cpp)

target_link_libraries(testRegion HackTM)
target_link_libraries(testMM HackTM)
target_link_libraries(testNode HackTM)
target_link_libraries(benchNode HackTM)

