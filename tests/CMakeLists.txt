add_executable(testMM testMM.cpp)
target_link_libraries(testMM hacktm)

add_executable(testNode testNode.cpp xbmlib.cpp)
target_link_libraries(testNode hacktm)

add_executable(testNode3D testNode3D.cpp xbmlib.cpp)
target_link_libraries(testNode3D hacktm)

add_executable(benchNode benchNode.cpp)
target_link_libraries(benchNode hacktm)

add_executable(showDendrites showDendrites.cpp)
target_link_libraries(showDendrites hacktm)
