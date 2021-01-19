//
// Created by Philipp Schmitz on 26.11.20.
//

#include "UniqueTable.h"
#include <iostream>


namespace ClassProject {

    UniqueTable::UniqueTable() {
        auto entry = new UniqueTableEntry(0, "0", 0, 0, 0);
        entries.insert(std::pair<int, UniqueTableEntry*>(0, entry));

        entry->setIsConst();
        entry = new UniqueTableEntry(1, "1", 1,1,1);
        entry->setIsConst();
        entries.insert(std::pair<int, UniqueTableEntry*>(1, entry));
    }

    void UniqueTable::insertEntry(UniqueTableEntry* entry) {
        entries.insert(std::pair<int, UniqueTableEntry*>(entries.size(), entry));
    }

    std::map<int, UniqueTableEntry*> UniqueTable::getTable() {
        return entries;
    }

    UniqueTableEntry* UniqueTable::getEntry(BDD_ID id) {
        return entries.find(id)->second;
    }

    void UniqueTable::printTable() {
        std::cout << "ID\t" << "| Label\t" <<  "| High\t" << "| Low\t" << "| TopVar\t" << std::endl;
        std::cout << "====================================" << std::endl;
        for(const auto & entry: entries) {
            std::cout << entry.second->getID() << "\t| " << entry.second->getLabel() <<"\t\t| " << entry.second->getHigh() << "\t\t| " << entry.second->getLow() << "\t\t| " << entry.second->getTopVar() << std::endl;
        }
    }


}



