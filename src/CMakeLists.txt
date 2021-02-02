
add_subdirectory(test)
add_subdirectory(bench)
add_subdirectory(verify)
add_subdirectory(reachable)

#Boost
find_package(Boost)

add_library(Manager Manager.cpp ManagerInterface.cpp UniqueTableEntry.cpp)
target_link_libraries(Manager ${Boost_LIBRARIES})
