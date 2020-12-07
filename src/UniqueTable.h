//
// Created by Philipp Schmitz on 26.11.20.
//

#ifndef VDSPROJECT_UNIQUETABLE_H
#define VDSPROJECT_UNIQUETABLE_H

#include "UniqueTableEntry.h"
#include <vector>

namespace ClassProject {


    class UniqueTable{
    public:

        UniqueTable();

        void insertEntry(UniqueTableEntry* entry);
        std::vector<UniqueTableEntry*> getTable();
        UniqueTableEntry* getEntry(BDD_ID id);
        void printTable();
    private:
        std::vector<UniqueTableEntry*> entries;
    };


}

#endif //VDSPROJECT_UNIQUETABLE_H
