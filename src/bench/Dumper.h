// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014

#ifndef DUMPER_H
#define DUMPER_H

#include <iostream>
#include "../Manager.h"
namespace ClassProject {
    
    class BddNodeDumper {
    public:
	virtual void dump(const BDD_ID& root,std::ostream& out)=0;
	virtual ~BddNodeDumper(){}
    };
    class BddDumper {
    public:
	virtual void dump(const BDD_ID& root,std::ostream& out)=0;
	virtual ~BddDumper(){}
    };


    class TextBddDumper: public BddDumper {
    public:
	TextBddDumper(Manager& mgr);
	virtual void dump(const BDD_ID& root,std::ostream& out );
	virtual ~TextBddDumper(){}
    private:
	Manager& mMgr;
    }; 

    class DotBddDumper: public BddDumper {
    public:
	DotBddDumper(Manager& mgr);
	virtual void dump(const BDD_ID& root,std::ostream& out );
	virtual ~DotBddDumper(){}
    private:
	Manager& mMgr;
    }; 
}
#endif

// Local Variables:
// mode: c++
// End:
