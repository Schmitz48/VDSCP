//
// Created by Philipp Schmitz on 26.11.20.
//

#include "UniqueTable.h"
#include <iostream>



namespace ClassProject {

    UniqueTable::UniqueTable() {
        auto entry = new UniqueTableEntry(0, "0", 0, 0, 0);
        entries.insert(std::pair<int, UniqueTableEntry*>(0, entry));
        std::vector<BDD_ID> Triple = {0,0,0};
        triple_entries.insert(std::pair<std::vector<BDD_ID>, int>(Triple, 0));
        entry->setIsConst();
        entry = new UniqueTableEntry(1, "1", 1,1,1);
        entry->setIsConst();
        entries.insert(std::pair<int, UniqueTableEntry*>(1, entry));
        std::vector<BDD_ID> Triple1 = {1,1,1};
        triple_entries.insert(std::pair<std::vector<BDD_ID>, int>(Triple1, 1));
    }

    void UniqueTable::insertEntry(UniqueTableEntry* entry) {
        int id = entries.size();
        entries.insert(std::pair<int, UniqueTableEntry*>(id, entry));
        std::vector<BDD_ID> Triple = {entry->getLow(),entry->getHigh(),entry->getTopVar()};
        triple_entries.insert(std::pair<std::vector<BDD_ID>, int>(Triple, id));
    }

    std::map<int, UniqueTableEntry*> UniqueTable::getTable() {
        return entries;
    }

    UniqueTableEntry* UniqueTable::getEntry(BDD_ID id) {
        return entries[id];
    }

    void UniqueTable::printTable() {
        std::cout << "ID\t" << "| Label\t" <<  "| High\t" << "| Low\t" << "| TopVar\t" << std::endl;
        std::cout << "====================================" << std::endl;
        for(const auto & entry: entries) {
            std::cout << entry.second->getID() << "\t| " << entry.second->getLabel() <<"\t\t| " << entry.second->getHigh() << "\t\t| " << entry.second->getLow() << "\t\t| " << entry.second->getTopVar() << std::endl;
        }
    }

    BDD_ID UniqueTable::getID(std::vector<BDD_ID> triple) {
        auto search = triple_entries.find(triple);
        if (search != triple_entries.end()) {
            return search->second;
        } else {
            return -1;
        }
    }

    bool UniqueTable::findID(std::vector<BDD_ID> triple) {
        if (triple_entries.find(triple) != triple_entries.end()) {
            return true;
        } else {
            return false;
        }
    }


}



