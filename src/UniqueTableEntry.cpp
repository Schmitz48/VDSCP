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
                                            top_var_(top_var),
                                            is_const_(false),
                                            is_var_(false){
    }

}