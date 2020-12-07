//
// Created by Philipp Schmitz on 26.11.20.
//

#include "UniqueTable.h"
#include <iostream>


namespace ClassProject {

    UniqueTable::UniqueTable() {
        auto entry = new UniqueTableEntry(0, "0", 0, 0, 0);
        entries.push_back(entry);
        entry->setIsConst();
        entry = new UniqueTableEntry(1, "1", 1,1,1);
        entry->setIsConst();
        entries.push_back(entry);
    }

    void UniqueTable::insertEntry(UniqueTableEntry* entry) {
        entries.push_back(entry);
    }

    std::vector<UniqueTableEntry*> UniqueTable::getTable() {
        return entries;
    }

    UniqueTableEntry* UniqueTable::getEntry(BDD_ID id) {
        return entries.at(id);
    }

    void UniqueTable::printTable() {
        std::cout << "ID\t" << "| Label\t" <<  "| High\t" << "| Low\t" << "| TopVar\t" << std::endl;
        std::cout << "====================================" << std::endl;
        for(const auto & entry: entries) {
            std::cout << entry->getID() << "\t| " << entry->getLabel() <<"\t\t| " << entry->getHigh() << "\t\t| " << entry->getLow() << "\t\t| " << entry->getTopVar() << std::endl;
        }
    }


}



