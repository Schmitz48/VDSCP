/*=============================================================================
    Written by Carolina P. Nogueira (2016)
=============================================================================*/
#include <boost/spirit/include/qi.hpp>
#include <vector>

#include <iostream>
#include <string>
#include <gtest/gtest.h>

/* Bench Circuit Manager Libray */
#include "bench_circuit_manager.hpp"

/* Bench Circuit to BDD Manager Libray */
#include "circuit_to_BDD_manager.hpp"

/* BDD libraries */
#include "../Manager.h"
#include "../ManagerInterface.h"

/* Benchmark Functions */
#include "BenchmarkLib.h"

int main(int argc, char* argv[])
{

    /* Number of arguments validation */
    if (2 > argc)
    {
        std::cout << "Must specify a filename!" << std::endl;
        return -1;
    }

    std::string bench_file = argv[1];
    
    /* Read the circuit from file and generate topological sorted circuit */
    bench_circuit_manager parsed_circuit(bench_file);
    
    /* Defining BDD Manager to be used */
    ClassProject::ManagerInterface* BDD_manager = new ClassProject::Manager();
    circuit_to_BDD_manager* circuit2BDD = new circuit_to_BDD_manager(BDD_manager);    
    
    /* Circuit to BDD conversion */
    double measured_utime, vm1, rss1, vm2, rss2;
  
    std::cout << "- Generating BDD from circuit...";
    process_mem_usage(vm1, rss1);
    measured_utime = userTime();
    circuit2BDD->GenerateBDD(parsed_circuit.GetSortedCircuit(),bench_file);
    measured_utime = userTime() - measured_utime;
    std::cout << " BDD generated successfully!" << std::endl << std::endl;
    
    /* Print the BDDs to txt and dot files */
    circuit2BDD->PrintBDD(parsed_circuit.GetListOfOutputLabels());

    std::cout << "**** Performance ****" << std::endl;
    /* Time Measurements */
    std::cout << " Runtime: " << measured_utime << std::endl;

    /* Memory Measurements */
    process_mem_usage(vm2, rss2);
    std::cout << " VM: " << vm2-vm1 << "; RSS: " << rss2-rss1 << endl << endl;
    return 0;
}
