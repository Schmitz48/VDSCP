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
        BDD_ID getHigh();
        BDD_ID getLow();
        BDD_ID getTopVar();
    private:
        BDD_ID id_;
        std::string label_;
        BDD_ID high_;
        BDD_ID low_;
        BDD_ID top_var_;
    };

}

#endif //VDSPROJECT_UNIQUETABLEENTRY_H
