/*=============================================================================
    Written by Carolina P. Nogueira (2016)
=============================================================================*/
#include "circuit_to_BDD_manager.hpp"


circuit_to_BDD_manager::circuit_to_BDD_manager(ClassProject::ManagerInterface *BDD_manager_iface){
    BDD_manager = BDD_manager_iface;
}

circuit_to_BDD_manager::~circuit_to_BDD_manager(){

}

void circuit_to_BDD_manager::GenerateBDD(list_of_circuit_t circuit, std::string benchmark_file){
    ClassProject::BDD_ID BDD_node;
    
    boost::filesystem::path pathToBenchFile(benchmark_file);
    if(!pathToBenchFile.has_filename()) throw std::runtime_error("circuit_to_BDD_manager::GenerateBDD: benchmark_file not specified");
    if(!boost::filesystem::exists(benchmark_file)) throw std::runtime_error("circuit_to_BDD_manager::GenerateBDD: benchmark_file doesn't exist");
    result_dir = "results_"+pathToBenchFile.stem().string();
    
    if(!(boost::filesystem::exists(result_dir)) && !boost::filesystem::create_directory(result_dir)){
		throw DirException("Unable to create directory 'result' for the output!");
	}

    std::ofstream bdd_out_file(result_dir+"/BNode_BDD.csv");
    
    /* In case of problem openning log file */
    if (!bdd_out_file.is_open()) {
        throw FileException("Unable to open Log File!"); 
    }   
    
    bdd_out_file << "BDD_ID,Bench Label" << std::endl;
    
    for(circuit_node_type & circuit_node : circuit) {       
        if(boost::iequals(circuit_node.gate_type, INPUT_GATE_T)){
            BDD_node = InputGate(circuit_node.label);
        }
        else if(boost::iequals(circuit_node.gate_type, NOT_GATE_T)){           
            BDD_node = NotGate(circuit_node.input_ID_list);
        }
        else if(boost::iequals(circuit_node.gate_type,AND_GATE_T)){
            BDD_node = AndGate(circuit_node.input_ID_list);
        }
        else if(boost::iequals(circuit_node.gate_type,OR_GATE_T)){
            BDD_node = OrGate(circuit_node.input_ID_list);
        }
        else if(boost::iequals(circuit_node.gate_type,NAND_GATE_T)){
            BDD_node = NandGate(circuit_node.input_ID_list);
        }
        else if(boost::iequals(circuit_node.gate_type,NOR_GATE_T)){
            BDD_node = NorGate(circuit_node.input_ID_list);
        }
        else if(boost::iequals(circuit_node.gate_type,XOR_GATE_T)){
            BDD_node = XorGate(circuit_node.input_ID_list);
        }
        else if(boost::iequals(circuit_node.gate_type,BUFFER_GATE_T)){
            BDD_node = circuitId2BDDid(*circuit_node.input_ID_list.begin());
        }

        /* OUTPUT or FLIP FLOP gates do not generate a BDD */
        if(!(boost::iequals(circuit_node.gate_type, OUTPUT_GATE_T) | boost::iequals(circuit_node.gate_type, FLIP_FLOP_GATE_T))){
            node2BDDTable.insert(std::pair<unique_ID_t,ClassProject::BDD_ID>(circuit_node.ID, BDD_node));
            label2BDD_ID_table.insert(std::pair<label_t, ClassProject::BDD_ID>(circuit_node.label, BDD_node));
            bdd_out_file << BDD_node << "," << circuit_node.label << std::endl;
        }
    }
    
    bdd_out_file.close();
}

/* -------------------
 * Auxiliar Functions
 * -------------------
 */

ClassProject::BDD_ID circuit_to_BDD_manager::circuitId2BDDid(unique_ID_t node){
    boost::unordered_map<unique_ID_t,ClassProject::BDD_ID>::const_iterator got;
    
    got = node2BDDTable.find(node);
        
    if (got!= node2BDDTable.end()){
        return got->second;
    }
    else{
        throw InexistentBDD_IDException("Destination node UUID is not part of the circuit graph!");
    }
}

/* --------------------------------
 * Boolean Logic Function Handlers
 * --------------------------------
 */

ClassProject::BDD_ID circuit_to_BDD_manager::InputGate(label_t label){
    return BDD_manager->createVar(label);
}

 
ClassProject::BDD_ID circuit_to_BDD_manager::NotGate(set_of_circuit_t inputNodes){
    unique_ID_t node=*inputNodes.begin();
    return BDD_manager->neg(circuitId2BDDid(node));
}          

ClassProject::BDD_ID circuit_to_BDD_manager::AndGate(set_of_circuit_t inputNodes){
    set_of_circuit_t::iterator it=inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;
   
    /* Get the ClassProject::BDD_ID of first elements */
    first_op = circuitId2BDDid(*it);
    inputNodes.erase(it);

    while(!inputNodes.empty()){
        it = inputNodes.begin();
        second_op = circuitId2BDDid(*it);
        inputNodes.erase(it);
        
        first_op = BDD_manager->and2(first_op, second_op);
    }
    
    /* Return the ClassProject::BDD_ID equivalent to the AND of all inputs */
    return first_op;
}

ClassProject::BDD_ID circuit_to_BDD_manager::OrGate(set_of_circuit_t inputNodes){
    set_of_circuit_t::iterator it=inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;
       
    /* Get the ClassProject::BDD_ID of first elements */
    first_op = circuitId2BDDid(*it);
    inputNodes.erase(it);

    while(!inputNodes.empty()){
        it = inputNodes.begin();
        second_op = circuitId2BDDid(*it);
        inputNodes.erase(it);
        
        first_op = BDD_manager->or2(first_op, second_op);
    }
    
    /* Return the ClassProject::BDD_ID equivalent to the OR of all inputs */
    return first_op;
}

ClassProject::BDD_ID circuit_to_BDD_manager::NandGate(set_of_circuit_t inputNodes){
    set_of_circuit_t::iterator it=inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;
    
    /* Get the ClassProject::BDD_ID of first elements */
    it = inputNodes.begin();
    first_op = circuitId2BDDid(*it);
    inputNodes.erase(it);

    if(inputNodes.size()==1){
        it = inputNodes.begin();
        second_op = circuitId2BDDid(*it);
        inputNodes.erase(it);
        
        /* Create the NAND BDD node for the first two elements */
        first_op = BDD_manager->nand2(first_op, second_op);
    }
    else{
        /* AND of all inputs, to use as the second operator of the NAND gate */
        second_op = AndGate(inputNodes);
        /* Create the NAND BDD node */
        first_op = BDD_manager->nand2(first_op, second_op);
    }
    
    /* Return the ClassProject::BDD_ID equivalent to the NAND of all inputs */
    return first_op;
}

ClassProject::BDD_ID circuit_to_BDD_manager::NorGate(set_of_circuit_t inputNodes){
    set_of_circuit_t::iterator it=inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;
    
    /* Get the ClassProject::BDD_ID of first elements */
    it = inputNodes.begin();
    first_op = circuitId2BDDid(*it);
    inputNodes.erase(it);

    if(inputNodes.size()==1){
        it = inputNodes.begin();
        second_op = circuitId2BDDid(*it);
        inputNodes.erase(it);
    
        /* Create the NOR BDD node for the first two elements */
        first_op = BDD_manager->nor2(first_op, second_op);
    }
    else{
        /* OR of all inputs, to use as the second operator of the NAND gate */
        second_op = OrGate(inputNodes);
        /* Create the NOR BDD node */
        first_op = BDD_manager->nor2(first_op, second_op);
    }
    
    /* Return the ClassProject::BDD_ID equivalent to the NOR of all inputs */
    return first_op;
}

ClassProject::BDD_ID circuit_to_BDD_manager::XorGate(set_of_circuit_t inputNodes){
    set_of_circuit_t::iterator it=inputNodes.begin();
    ClassProject::BDD_ID first_op, second_op;
    
    /* Get the ClassProject::BDD_ID of first elements */
    first_op = circuitId2BDDid(*it);
    inputNodes.erase(it);

    while(!inputNodes.empty()){
        it = inputNodes.begin();
        second_op = circuitId2BDDid(*it);
        inputNodes.erase(it);
        
        first_op = BDD_manager->xor2(first_op, second_op);
    }
    
    /* Return the ClassProject::BDD_ID equivalent to the XOR of all inputs */
    return first_op;
}   
/* --------------------------------
 *       Printing Functions
 * --------------------------------
 */   
void circuit_to_BDD_manager::PrintBDD(std::set<label_t> set_of_output_labels){
	std::unordered_map<label_t, ClassProject::BDD_ID>::iterator got;
	
	ClassProject::DotBddDumper* BDD_dot = new ClassProject::DotBddDumper((ClassProject::Manager&)*BDD_manager);
	ClassProject::TextBddDumper* BDD_txt = new ClassProject::TextBddDumper((ClassProject::Manager&)*BDD_manager);
	std::string dot_file_name, txt_file_name;	 
	
	if ((!(boost::filesystem::exists(result_dir+"/txt"))&!(boost::filesystem::create_directory(result_dir+"/txt")))
		& (!(boost::filesystem::exists(result_dir+"/dot")) & !(boost::filesystem::create_directory(result_dir+"/dot")))){
		throw DirException("Unable to create directories 'txt' and 'dot' for the output!"); 
	}
	
    for(std::set<label_t>::const_iterator it = set_of_output_labels.begin(); it != set_of_output_labels.end(); it++){
		
		/* Search BDD_ID of output */  
		got = label2BDD_ID_table.find(*it);        

		if (got  != label2BDD_ID_table.end()){
			dot_file_name = result_dir+"/dot/"+std::string(*it)+".dot";
			txt_file_name = result_dir+"/txt/"+std::string(*it)+".txt";
			
			/* Create output file */
			std::ofstream bdd_out_dot_file(dot_file_name);
			std::ofstream bdd_out_txt_file(txt_file_name);

			/* In case of problem openning log file */
			if (!bdd_out_dot_file.is_open() | !bdd_out_txt_file.is_open()) {
				throw FileException("Unable to open Log File!"); 
			}   
    
			
			/* Dump result into files */
			BDD_dot->dump(got->second, bdd_out_dot_file);
			BDD_txt->dump(got->second, bdd_out_txt_file);

			bdd_out_dot_file.close();
			bdd_out_txt_file.close();			
		}//if
		else{
			throw InexistentBDDLabelException("Destination node UUID is not part of the circuit graph!");
		}
    }
}


