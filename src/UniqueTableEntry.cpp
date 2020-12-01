//
// Created by Philipp Schmitz on 26.11.20.
//

#include "UniqueTableEntry.h"

#include <utility>

namespace ClassProject {

    UniqueTableEntry::UniqueTableEntry(BDD_ID id,
                                       std::string label,
                                       BDD_ID high,
                                       BDD_ID low,
                                       BDD_ID top_var):
                                            id_(id),
                                            label_(std::move(label)),
                                            high_(high),
                                            low_(low),
                                            top_var_(top_var) {

    }

    BDD_ID& UniqueTableEntry::getID(){
        return id_;
    }

    std::string UniqueTableEntry::getLabel(){
        return label_;
    }

    BDD_ID UniqueTableEntry::getHigh(){
        return high_;
    }

    BDD_ID UniqueTableEntry::getLow(){
        return low_;
    }

    BDD_ID UniqueTableEntry::getTopVar(){
        return top_var_;
    }

}