/*=============================================================================
    Written by Carolina P. Nogueira (2016)
=============================================================================*/
#include "bench_circuit_manager.hpp"

bench_circuit_manager::bench_circuit_manager(std::string bench_file){

	ID_counter = 0;

	if(ParseFile(bench_file)){
		/* Based on the list of output labels, generate the corresponding circuit */
		std::cout << "- Creating circuit from bench nodes... ";
		CreateCircuitFromOutputList();
		std::cout << "Done!" << std::endl;
		
		/* Sort the circuit */
		std::cout << "- Topologically sorting the circuit... ";
		TopologicalSortKahnsAlgorithm();
		std::cout << "Done!" << std::endl;
		
		labels2node_table.clear();
		labels2uuid_table.clear();
		uuid2circuitNode_table.clear();
		set_of_output_labels.clear(); 
		set_of_flipflop_labels.clear();
		set_of_circuit_OUT.clear();
        set_of_circuit_IN.clear();
	}
	else{
		throw SyntaxException("Please check bench file syntax!");
	}
}

bench_circuit_manager::~bench_circuit_manager(){

}

/* --------------- 
 * Print Functions 
 * ---------------
 */
void bench_circuit_manager::PrintLabelsTable(){
    std::unordered_map<label_t, bench_node_t>::const_iterator it_label;

    std::cout << "============ [BEGIN] Table of Labels and Nodes ============" << std::endl;
    std::cout << std::endl << "-----" << std::endl;
    for(it_label = labels2node_table.begin(); it_label != labels2node_table.end(); it_label++){
        std::cout << std::endl << "Label: " << it_label->first << std::endl;
        std::cout << "Node Information:" << std::endl;
        std::cout << "\tNode Label: " << it_label->second.label     <<std::endl;
        std::cout << "\tGate Type: "  << it_label->second.gate_type <<std::endl;
        std::cout << "\tInputs: " ;
        for (std::vector<std::string>::const_iterator i = it_label->second.input_node_list.begin(); i != it_label->second.input_node_list.end(); ++i)
            std::cout << *i << ' ';
        std::cout << std::endl << "-----" << std::endl;
    }
    std::cout << "============ [END] Table of Labels and Nodes ============" << std::endl;
}

void bench_circuit_manager::PrintUUIDCircuitTable(){
    boost::unordered_map<unique_ID_t, circuit_node_type>::const_iterator it_uuid;

    std::cout << "============ [BEGIN] Table of Labels and Nodes ============" << std::endl;
    std::cout << std::endl << "-----" << std::endl;
    for(it_uuid = uuid2circuitNode_table.begin(); it_uuid != uuid2circuitNode_table.end(); it_uuid++){
        std::cout << std::endl << "UUID: " << it_uuid->first << std::endl;
        std::cout << "Node Information:" << std::endl;
        std::cout << "\tNode Circuit Label: " << it_uuid->second.label     <<std::endl;
        std::cout << "\tGate Type: "  << it_uuid->second.gate_type <<std::endl;
        std::cout << "\tInputs: " ;
        for (set_of_circuit_t::const_iterator i = it_uuid->second.input_ID_list.begin(); i != it_uuid->second.input_ID_list.end(); ++i)
            std::cout << *i << ' ';
        std::cout << std::endl << "-----" << std::endl;
        std::cout << "\tOutputs: " ;
        for (set_of_circuit_t::const_iterator i = it_uuid->second.output_ID_list.begin(); i != it_uuid->second.output_ID_list.end(); ++i)
            std::cout << *i << ' ';
        std::cout << std::endl << "-----" << std::endl;
    }
    std::cout << "============ [END] Table of Labels and Nodes ============" << std::endl;
}

void bench_circuit_manager::PrintOutputList(){
    std::set<label_t>::const_iterator it;

    std::cout << std::endl << "============ [BEGIN] List of Outputs ============" << std::endl << std::endl;
    std::cout << std::endl << "List of output labels: ";
    for(it = set_of_output_labels.begin(); it != set_of_output_labels.end(); it++){
        std::cout<< (*it) << " -> ";
    }
    std::cout << "end;"<< std::endl;
    std::cout << std::endl << "============ [END] List of Outputs ============" << std::endl;
}

void bench_circuit_manager::PrintLabels2UUIDTable(void){
    std::unordered_map<label_t, unique_ID_t> ::const_iterator it_label;

    std::cout << "============ [BEGIN] Table of Labels and UUIDs ============" << std::endl;
    std::cout << std::endl << "-----" << std::endl;
    for(it_label = labels2uuid_table.begin(); it_label != labels2uuid_table.end(); it_label++){
        std::cout << std::endl << "Label: " << it_label->first << std::endl;
        std::cout << std::endl << "UUID: " << it_label->second << std::endl;
        std::cout << std::endl << "------------------------------" << std::endl;
    }
    std::cout << "============ [END] Table of Labels and UUIDs ============" << std::endl;
}

void bench_circuit_manager::PrintCircuit(unique_ID_t circuit_ID, int indent){
        boost::unordered_map<unique_ID_t, circuit_node_type>::const_iterator got;
        circuit_node_type node;
                
        got = (uuid2circuitNode_table).find(circuit_ID);
        if (got  != uuid2circuitNode_table.end()){ 
            node = got->second;
            std::cout << std::string(indent, ' ') << "Node ID: " << node.ID << std::endl;
            std::cout << std::string(indent, ' ') << "Label: "   << node.label << std::endl;
            std::cout << std::string(indent, ' ') << "Type: "    << node.gate_type << std::endl;
 
            std::cout << std::string(indent, ' ') << "Input List: " << std::endl;
            for (set_of_circuit_t::const_iterator i = node.input_ID_list.begin(); i != node.input_ID_list.end(); ++i)
                std::cout << std::string(indent, ' ') << "    " << *i << std::endl;

            std::cout << std::string(indent, ' ') << "Output List: " << std::endl;
            for (set_of_circuit_t::const_iterator i = node.output_ID_list.begin(); i != node.output_ID_list.end(); ++i)
                std::cout << std::string(indent, ' ') << "    " << *i << std::endl;

            indent=indent+4;                
            std::cout << std::string(indent, ' ') << "--------------" << std::endl;
            for (set_of_circuit_t::const_iterator i = node.input_ID_list.begin(); i != node.input_ID_list.end(); ++i)
                PrintCircuit(*i,indent);                            
            std::cout << std::string(indent, ' ') << "--------------" << std::endl;
        }
}

void bench_circuit_manager::PrintCircuitByLabel(label_t node_label){
        std::unordered_map<label_t, unique_ID_t>::const_iterator got;
                
        got = (labels2uuid_table).find(node_label);
        if (got  != labels2uuid_table.end()){ 
            PrintCircuit(got->second,0);
        }
        else{
            throw InexistentLabelException("There is no mapping from this label to a circuit node.");
        }

}


void bench_circuit_manager::PrintCircuitsOfOutputSet(void){
    for(set_of_circuit_t::const_iterator it = set_of_circuit_OUT.begin(); it != set_of_circuit_OUT.end(); it++){
        PrintCircuit(*it, 0);
    }
}

void bench_circuit_manager::PrintSortedCircuitList(){
    list_of_circuit_t::const_iterator it;

    std::cout << std::endl << "============ [BEGIN] List of Sorted Circuit Nodes ============" << std::endl << std::endl;
    std::cout << std::endl << "List of Sorted Circuit Nodes labels: ";
    
    list_of_circuit_t ordered_circuit = this->GetSortedCircuit();
    
    
    for(it = ordered_circuit.begin(); it != ordered_circuit.end(); it++){
        std::cout<< (*it).ID << " -> ";
    }
    std::cout << "end;"<< std::endl;
    std::cout << std::endl << "============ [END] List of Sorted Circuit Nodes ============" << std::endl;
}

/* ---------------- 
 * Get functions
 * ----------------
 */
 
set_of_circuit_t bench_circuit_manager::GetListOfCircuitsInputs(){
    return set_of_circuit_IN;
}
 
 
set_of_circuit_t bench_circuit_manager::GetListOfCircuitsOutputs(){
    return set_of_circuit_OUT;
}

std::set<label_t> bench_circuit_manager::GetListOfOutputLabels(){
	return outputs;
}


circuit_node_type bench_circuit_manager::GetCircuitNode(unique_ID_t circuit_node_uuid){
    /* Iterator for the uuid2circuitNode_table table */
    boost::unordered_map<unique_ID_t, circuit_node_type>::const_iterator got;

    /* Search the node and add the UUID of the current node to this node OUTPUT_LIST */
    got = uuid2circuitNode_table.find(circuit_node_uuid);
        
    if (got  != uuid2circuitNode_table.end()){
        return got->second;
    }//if
    else{
        throw InexistentUUIDException("There is no mapping from this unique_ID_t to a circuit_node_type.");
    }
}

boost::unordered_map<unique_ID_t, circuit_node_type>  bench_circuit_manager::GetCircuit(void){
    return uuid2circuitNode_table;
}

list_of_circuit_t bench_circuit_manager::GetSortedCircuit(void){
    return sorted_circuit;
}

/* --------------- 
 * Read File Functions 
 * ---------------
 */ 
bool bench_circuit_manager::ParseFile(std::string bench_file){
	std::cout << std::endl << "- Reading bench format file... ";
    std::ifstream in(bench_file);
	
    if (!in.is_open())
    {
		throw OpenFileException("Problems acessing file " + bench_file);
    }
    
    in.unsetf(std::ios::skipws);
    std::cout << "Done!" << std::endl;
    
    /* Declaring file interators */
    boost::spirit::istream_iterator first(in), last;   
    
    /* Declaring skip parser to be used */
    skip_p::skip_grammar<boost::spirit::istream_iterator> skip;

    /* Declaring bench format grammar */
    bench_format::bench_parser<boost::spirit::istream_iterator> bench_grammar_parser;
        
    /* Stored result after parsing a file line */
    bench_format::bench_node_type parsed_bench_node;
    
    /* Effectively parsing the file. The parser returns a benchnode to be added to the labels table */
    std::cout << "- Parsing input file '"<< bench_file << "'... ";
    bool successfully_parsed;
    do
    {
        successfully_parsed = phrase_parse(first, last, bench_grammar_parser, skip, parsed_bench_node);
        if (!successfully_parsed) {
            std::cout << "Failed parsing input file at: " << std::string(first,last) <<std::endl;
            return false;
        }
        else{
            /* Add the successfully read bench node into the labels table */
            AddToLabelsTable(parsed_bench_node);
        }
        parsed_bench_node=bench_format::bench_node_type();
    }while(first!=last); /* Operations are performed until it is EOF */
    std::cout << "Done!" << std::endl;
    
	return true;
}

/* ---------------- 
 * Insert functions
 * ----------------
 */
bool bench_circuit_manager::AddToLabelsTable(bench_node_t bench_node){
    bool returnValue;
    label_t search_label;
    /*
     * Output nodes have to be handle a bit different, since they'll have
     *  the same label as another gate. Mappings require unique element
     *  identifiers. By convention, OUTPUT gates will be referenced by its
     *  original label concatenated with the string "OUTPUT" to differenciate 
     *  from the other gate that has the same label.
    */   
    if (boost::iequals(bench_node.gate_type,OUTPUT_GATE_T)){
        /* If the gate is an output gate, it must be included into the set_of_output_labels */
        set_of_output_labels.insert(bench_node.label);
        search_label = bench_node.label + bench_node.gate_type;        
    }
    else{
        search_label = bench_node.label;
    }
    
    /* Iterator to search into the labels2node table */
    std::unordered_map<label_t, bench_node_t>::const_iterator got;

    /* Search node into the labels_table */
    got = (labels2node_table).find(search_label);
        
    if (got  != labels2node_table.end()){ 
        /* 
         * If the iterator is not pointing to the end of the labels_table, 
         * means that already exist another node with the same label 
         */
        returnValue = false;
    }
    else{ 
        /*
         * Otherwise add bench_node to the labels_table
         */
        if (boost::iequals(bench_node.gate_type, FLIP_FLOP_GATE_T)){
            /* 
             * If it is a flip flop, we have to add two nodes:
             *    One that will be the output node;
             *    Another one as input node.
             *  For the searching part it is not necessary to differentiate it,
             *      since for each existing flip flop in the circuit we will add
             *      to the labels table one node that is an INPUT with the same 
             *      label as the flip flop, and another one with the "FLIP_FLOP_GATE_T"
             *      string concatenated. So if we search for one of them, it is 
             *      enough to check whether the node exists or not.
             */

            set_of_flipflop_labels.insert(search_label);
            labels2node_table.insert(std::pair<label_t, bench_node_t>(search_label+FLIP_FLOP_GATE_T, bench_node));
            
            bench_node.gate_type = INPUT_GATE_T;   
            bench_node.input_node_list.clear();

        }

        labels2node_table.insert(std::pair<label_t, bench_node_t>(search_label, bench_node));
        
        returnValue =  true;
    }
    /* returnValue is
     *      true  -> Node successfully added 
     *      false -> Node already exist and could not be added
     */
   
    return returnValue;    
}

unique_ID_t bench_circuit_manager::FindOrAddToCircuit(bench_node_t bench_node){
    unique_ID_t CircuitNodeID;
    std::unordered_map<label_t, unique_ID_t>::const_iterator got;
    circuit_node_type new_circuit_node;
    label_t search_label;
    
    /* Check if it isn't an output node */
    if (boost::iequals(bench_node.gate_type,OUTPUT_GATE_T) || boost::iequals(bench_node.gate_type,FLIP_FLOP_GATE_T)){
        search_label = bench_node.label + bench_node.gate_type;        
    }
    else{
        search_label = bench_node.label;
    }
      
    /* Search node into the labels to UUID table */
    got = (labels2uuid_table).find(search_label);
    
    if (got  != labels2uuid_table.end()){ 
        /* 
         * If the iterator is not pointing to the end of the labels_table, 
         *   means that already exist another node with the same label. Then
         *   simply returns the UUID from the node.
         */
         CircuitNodeID = (got->second);
    }
    else{ 
        /*
         * Otherwise add bench_node to the labels_table and returns its UUID
         */
         new_circuit_node = BenchNode2CircuitNode(bench_node);
         CircuitNodeID = new_circuit_node.ID;
         labels2uuid_table.insert(std::pair<label_t, unique_ID_t>(search_label, CircuitNodeID));
         uuid2circuitNode_table.insert(std::pair<unique_ID_t, circuit_node_type>(CircuitNodeID, new_circuit_node));
    }
    return CircuitNodeID;
}


unique_ID_t bench_circuit_manager::FindOrAddToCircuitbyLabel(label_t node_label){
    std::unordered_map<label_t, bench_node_t>::const_iterator got;   
    got = (labels2node_table).find(node_label);
        
    if (got  != labels2node_table.end()){ 
        return (FindOrAddToCircuit(got->second));
    }
    else{
        throw InexistentLabelException("There is no mapping from this label to a node.");
    }
}

/* -------------------- 
 * Convertion functions
 * --------------------
 */
circuit_node_type bench_circuit_manager::BenchNode2CircuitNode(bench_node_t bench_node){
    circuit_node_type new_circuit_node;
    unique_ID_t uuid_input, new_ID;
    
    /* Iterator for the uuid2circuitNode_table table */
    boost::unordered_map<unique_ID_t, circuit_node_type>::iterator got;
    
    /* Generate an unique identifier */
    // by mohammad
    //new_circuit_node.ID = generator();
    new_circuit_node.ID = ID_counter;
    ID_counter ++;
    new_ID = new_circuit_node.ID;
    
    /* Label of the node remains the same */
    new_circuit_node.label = bench_node.label;
    
    /* Gate type also remains the same */
    new_circuit_node.gate_type = bench_node.gate_type;
    
    /* If it is not an INPUT gate, we have to recursivelly iterate to get the UUID of the inputs */
    if(!boost::iequals(new_circuit_node.gate_type,INPUT_GATE_T)){
        
        if(boost::iequals(new_circuit_node.gate_type,OUTPUT_GATE_T)){
            uuid_input = FindOrAddToCircuitbyLabel(bench_node.label);
            new_circuit_node.input_ID_list.insert(uuid_input);
                
            /* Search the node and add the UUID of the current node to this node OUTPUT_LIST */
            got = (uuid2circuitNode_table).find(uuid_input);
        
            if (got  != uuid2circuitNode_table.end()){ 
                got->second.output_ID_list.insert(new_ID);
            }//if
        }
        else{	
            for(std::string & input_node : bench_node.input_node_list) {
                /* Adds the UIID to the list of inputs */
                uuid_input = FindOrAddToCircuitbyLabel(input_node);
                new_circuit_node.input_ID_list.insert(uuid_input);
                
                /* Search the node and add the UUID of the current node to this node OUTPUT_LIST */
                got = (uuid2circuitNode_table).find(uuid_input);
        
                if (got  != uuid2circuitNode_table.end()){ 
                    got->second.output_ID_list.insert(new_ID);
                }//if
            }//for 
        }
        
    } //if
    else {
        /* Add node UUID to input list */
        set_of_circuit_IN.insert(new_ID);
    }
    
    return new_circuit_node;

} /* benchNode2CircuitNode */

/* ------------------------ 
 * Create circuit functions
 * ------------------------
 */

void bench_circuit_manager::CreateCircuitFromOutputList(){
	std::set<label_t>::const_iterator it;
	std::unordered_map<label_t, unique_ID_t>::const_iterator got_uid;
	boost::unordered_map<unique_ID_t, circuit_node_type>::const_iterator got_node;
	
	
    for(it = set_of_output_labels.begin(); it != set_of_output_labels.end(); it++){
        CreateCircuitbyLabel(*it+OUTPUT_GATE_T);

    }
    /* After checking the output list, we do have to create also the circuit parts from the Flip Flop lists */
    for(it = set_of_flipflop_labels.begin(); it != set_of_flipflop_labels.end(); it++){
        CreateCircuitbyLabel(*it+FLIP_FLOP_GATE_T);
    }
    
    for(it = set_of_flipflop_labels.begin(); it != set_of_flipflop_labels.end(); it++){
		got_uid = labels2uuid_table.find(*it+FLIP_FLOP_GATE_T);
		got_node = uuid2circuitNode_table.find(got_uid->second);
		
		got_node = uuid2circuitNode_table.find(*(got_node->second).input_ID_list.begin());
		outputs.insert((got_node->second).label);
	}
	outputs.insert(set_of_output_labels.begin(),set_of_output_labels.end());
}

void bench_circuit_manager::CreateCircuitbyLabel(label_t bnode_label){
    unique_ID_t new_circuit;
    new_circuit=FindOrAddToCircuitbyLabel(bnode_label);
    set_of_circuit_OUT.insert(new_circuit);
}



/* -----------------------------
 * Topological Sort Algorithms
 * -----------------------------
 */
void bench_circuit_manager::TopologicalSortKahnsAlgorithm(){
    set_of_circuit_t nodes_without_outgoing_edges = GetListOfCircuitsOutputs();
    set_of_circuit_t::iterator it;
    
    circuit_node_type node,tmp_node;
    
    while(!nodes_without_outgoing_edges.empty()){
        it = nodes_without_outgoing_edges.begin();
        /* Always pick the first element of the list of nodes without incomming edges */
        node = GetCircuitNode(*it);              
        nodes_without_outgoing_edges.erase(it);  

        /* Nodes are added alway to the end of the list */
        sorted_circuit.push_front(node);       
        for(set_of_circuit_t::const_iterator it_input_nodes = node.input_ID_list.begin(); it_input_nodes != node.input_ID_list.end(); it_input_nodes++){
            if (IsUniqueOutgoingEdge(*it_input_nodes,node.ID)){
                nodes_without_outgoing_edges.insert(*it_input_nodes);
            }
        }
    }

    if (sorted_circuit.size() != uuid2circuitNode_table.size()){
        throw CicleException("The circuit must be cycle free!");
    }
}


/* -----------------------------
 * Auxiliar functions for
 * Topological Sort Algorithms
 * -----------------------------
 */

bool bench_circuit_manager::IsUniqueOutgoingEdge(unique_ID_t src_node, unique_ID_t dst_node){
    boost::unordered_map<unique_ID_t, circuit_node_type>::iterator got;
    
    /* Search source node in the circuit graph */  
    got = uuid2circuitNode_table.find(src_node);
        
    if (got  != uuid2circuitNode_table.end()){
        /* Remove outgoing edge to source node */
        (got->second).output_ID_list.erase((got->second).output_ID_list.find(dst_node));
        return (got->second).output_ID_list.empty();
    }//if
    else{
        throw InexistentUUIDException("Destination node UUID is not part of the circuit graph!");
    }
}
