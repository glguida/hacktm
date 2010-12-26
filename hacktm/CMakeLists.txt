include_directories(..)
add_library(hacktm HackTM.cpp Space.cpp Introspection.cpp rnd_normal.cpp
		   ProximalDendrite.cpp SpatialPooler.cpp 
	           CISpace.cpp CellsState.cpp DendriteSegment.cpp Cell.cpp TemporalPooler.cpp)
		

