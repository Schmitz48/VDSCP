//
// Created by Philipp Schmitz on 26.11.20.
//

#include "UniqueTable.h"


namespace ClassProject {

    UniqueTable::UniqueTable() {
        entries.push_back(new UniqueTableEntry(0, "False", 0, 0, 0));
        entries.push_back(new UniqueTableEntry(1, "True", 1,1,1));
    }

    void UniqueTable::insertEntry(UniqueTableEntry* entry) {
        entries.push_back(entry);
    }

    std::vector<UniqueTableEntry*> UniqueTable::getTable() {
        return entries;
    }


}



