//
// Created by Philipp Schmitz on 26.11.20.
//

#ifndef VDSPROJECT_UNIQUETABLEENTRY_H
#define VDSPROJECT_UNIQUETABLEENTRY_H

#include "ManagerInterface.h"
#include <vector>

namespace ClassProject {

    //!  UniqueTableEntry
    /*!
    A unique table entry contains information about the individual entry,
    i.e, id, label, high successor, low successor and the top variable.
    This information can be accessed by the getter functions of this class.
    Additionally, the two boolean flags is_var_ and is_const_ can be read.
    The triple {high successor, low successor, top variable} is to determine whether
    the entry is unique or not and can also be accessed by a getter function.
    */

    class UniqueTableEntry{
    public:
        UniqueTableEntry(BDD_ID id,
                std::string label,
                BDD_ID high,
                BDD_ID low,
                BDD_ID top_var);

        //Getter
        const BDD_ID& getID();
        const std::string getLabel();
        const BDD_ID getHigh() const;
        const BDD_ID getLow() const;
        const BDD_ID getTopVar() const;
        bool getIsVar() const;
        bool getIsConst() const;
        std::vector<BDD_ID> getTriple();


        //Setter

        void setIsVar();
        void setIsConst();
        void setTriple();

    //private:
        const BDD_ID id_;
        const std::string label_;
        const BDD_ID high_;
        const BDD_ID low_;
        const BDD_ID top_var_;
        bool is_var_;
        bool is_const_;
        std::vector<BDD_ID> triple_;
    };

}

#endif //VDSPROJECT_UNIQUETABLEENTRY_H
