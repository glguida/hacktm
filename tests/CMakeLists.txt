add_executable(testSphere testSphere.cpp)
add_executable(testRegion testRegion.cpp)
add_executable(testMM testMM.cpp)

target_link_libraries(testSphere metric HackTM)
target_link_libraries(testRegion metric HackTM)
target_link_libraries(testMM metric HackTM)
