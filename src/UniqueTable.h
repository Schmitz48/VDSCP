//
// Created by Philipp Schmitz on 26.11.20.
//

#ifndef VDSPROJECT_UNIQUETABLE_H
#define VDSPROJECT_UNIQUETABLE_H

#include "UniqueTableEntry.h"
#include "../3rdParty/robin-map/include/tsl/robin_map.h"
#include <map>

namespace ClassProject {

    //!  UniqueTable
    /*!
    The unique table contains a vector of unique table entries.
    Each entry contains information about a row in the table.
    The class provides a function to insert a new entry to the table
    as well as getter functions to access either the whole table or a
    specific entry at a given ID. The function printTable() can be used to
    visualize its content for debugging purposes.
    */
    class UniqueTable{
    public:

        UniqueTable();
        //! adds an entry to the table
        void insertEntry(UniqueTableEntry* entry);
        //! returns all entries
        tsl::robin_map<int, UniqueTableEntry*> getTable();
        //! returns a specified entry
        UniqueTableEntry* getEntry(BDD_ID id);
        //! visualizes the table in the command line
        void printTable();
    private:
        tsl::robin_map<int, UniqueTableEntry*> entries;

    };


}

#endif //VDSPROJECT_UNIQUETABLE_H
