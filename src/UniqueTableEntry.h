//
// Created by Philipp Schmitz on 26.11.20.
//

#ifndef VDSPROJECT_UNIQUETABLEENTRY_H
#define VDSPROJECT_UNIQUETABLEENTRY_H

#include "ManagerInterface.h"

namespace ClassProject {

    class UniqueTableEntry{
    public:
        UniqueTableEntry(BDD_ID id,
                std::string label,
                BDD_ID high,
                BDD_ID low,
                BDD_ID top_var);

        //Getter
        BDD_ID& getID();
        std::string getLabel();
        BDD_ID getHigh() const;
        BDD_ID getLow() const;
        BDD_ID getTopVar() const;
        bool getIsVar() const;
        bool getIsConst() const;
        std::set<BDD_ID> getTriple();


        //Setter

        void setIsVar();
        void setIsConst();
        void setTriple();

    private:
        BDD_ID id_;
        std::string label_;
        BDD_ID high_;
        BDD_ID low_;
        BDD_ID top_var_;
        bool is_var_;
        bool is_const_;
        std::set<BDD_ID> triple;
    };

}

#endif //VDSPROJECT_UNIQUETABLEENTRY_H
