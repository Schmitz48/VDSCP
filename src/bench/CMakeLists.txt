add_library(Benchmark
        bench_circuit_manager.cpp
        BenchmarkLib.cpp
        circuit_to_BDD_manager.cpp
        Dumper.cpp
        bench_grammar.hpp
        skip_parser.hpp)

#Boost
find_package(Boost COMPONENTS system filesystem REQUIRED)

#Exectuable
add_executable(VDSProject_bench main_bench.cpp)
target_link_libraries(VDSProject_bench Manager)
target_link_libraries(VDSProject_bench Benchmark)
target_link_libraries(VDSProject_bench ${Boost_LIBRARIES})


