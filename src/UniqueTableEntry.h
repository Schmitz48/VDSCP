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
    */

    class UniqueTableEntry{
    public:
        UniqueTableEntry(BDD_ID id,
                std::string label,
                BDD_ID high,
                BDD_ID low,
                BDD_ID top_var);
        //Constructor for complement edges
        UniqueTableEntry(BDD_ID id, UniqueTableEntry* entry, bool complement);

        BDD_ID id_;
        std::string label_;
        BDD_ID high_;
        BDD_ID low_;
        BDD_ID top_var_;
        bool is_var_;
        bool is_const_;
        bool complement_;
        std::vector<BDD_ID> triple_;
    };

}

#endif //VDSPROJECT_UNIQUETABLEENTRY_H
