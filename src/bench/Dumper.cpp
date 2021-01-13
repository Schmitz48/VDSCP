#include "Dumper.h"

ClassProject::TextBddDumper::TextBddDumper(Manager& mgr) : mMgr(mgr){}

void ClassProject::TextBddDumper::dump(const BDD_ID& root,std::ostream& out ) {
    std::set<BDD_ID> nodes;
    mMgr.findNodes(root,nodes);
    for(std::set<BDD_ID>::reverse_iterator it=nodes.rbegin();it!=nodes.rend();++it){
	if (*it==mMgr.True()) {
	    out<<"Terminal Node: 1\n";
	} else if(*it==mMgr.False())   {
	    out<<"Terminal Node: 0\n";
	} else {
       
	    out<<"Variable Node: "<<(*it)<<" Top Var Id: "<<mMgr.topVar(*it)<<" Top Var Name: "<<mMgr.getTopVarName(mMgr.topVar(*it))<<" Low: "<< mMgr.coFactorFalse(*it)<< " High: "<<mMgr.coFactorTrue(*it)<<"\n";
	}
    }
}
ClassProject::DotBddDumper::DotBddDumper(Manager& mgr) : mMgr(mgr){}

void ClassProject::DotBddDumper::dump(const BDD_ID& root,std::ostream& out ) {
    out<< "digraph BDD {\n";
    out<< "center = true;\n";
    out<< "{ rank = same; { node [style=invis]; \"T\" };\n";
    out<< " { node [shape=box,fontsize=12]; \"0\"; }\n";
    out<< "  { node [shape=box,fontsize=12]; \"1\"; }\n}\n";
    std::set<BDD_ID> vars;
    std::set<BDD_ID> nodes;
    mMgr.findVars(root,vars);
    mMgr.findNodes(root,nodes);
    for( std::set<BDD_ID>::iterator it_vars=vars.begin();it_vars != vars.end();
	 ++it_vars ) {
	out<<"{ rank=same; { node [shape=plaintext,fontname=\"Times Italic\",fontsize=12] \""<<mMgr.getTopVarName(*it_vars)<<"\" };";
	for( std::set<BDD_ID>::iterator it_nodes=nodes.begin();
	     it_nodes != nodes.end();++it_nodes) {
	    if ( mMgr.topVar(*it_nodes)==*it_vars ) {
		out<<"\""<<*it_nodes<<"\";";
	    }
	}
	out<<"}\n";
    } 
    out<<"edge [style = invis]; {"; 
    for( std::set<BDD_ID>::iterator it_vars=vars.begin();it_vars != vars.end();
	 ++it_vars ) {
	out<<"\""<<mMgr.getTopVarName(*it_vars)<<"\" -> ";
    }
    out<< "\"T\"; }\n";
    for( std::set<BDD_ID>::iterator it=nodes.begin();it!= nodes.end(); ++it ) {
	if( !mMgr.isConstant(*it)) {
	    out<<"\""<<*it<<"\" -> \""<<mMgr.coFactorTrue(*it)<<"\" [style=solid,arrowsize=\".75\"];\n";
	    out<<"\""<<*it<<"\" -> \""<<mMgr.coFactorFalse(*it)<<"\" [style=dashed,arrowsize=\".75\"];\n";

	}
    }
    out<< "}\n";
  
    

  
}


// Local Variables:
// mode: c++
// End:
