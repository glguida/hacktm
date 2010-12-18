add_executable(testMM testMM.cpp)
target_link_libraries(testMM hacktm)

add_executable(testNode testNode.cpp)
target_link_libraries(testNode hacktm)

add_executable(benchNode benchNode.cpp)
target_link_libraries(benchNode hacktm)

#add_executable(testRegion testRegion.cpp)
#target_link_libraries(testRegion hacktm)
