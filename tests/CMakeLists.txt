add_executable(testSphere testSphere.cpp)
add_executable(testRegion testRegion.cpp)
add_executable(testMM testMM.cpp)
add_executable(testMisc test.cpp)
add_executable(testNode testNode.cpp)

target_link_libraries(testSphere metric HackTM)
target_link_libraries(testRegion metric HackTM)
target_link_libraries(testMM metric HackTM)
target_link_libraries(testMisc metric HackTM)
target_link_libraries(testNode metric HackTM)

