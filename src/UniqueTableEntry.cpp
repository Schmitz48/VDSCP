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
        setTriple();
    }

    const BDD_ID& UniqueTableEntry::getID(){
        return id_;
    }

    const std::string UniqueTableEntry::getLabel(){
        return label_;
    }

    const BDD_ID UniqueTableEntry::getHigh() const{
        return high_;
    }

    const BDD_ID UniqueTableEntry::getLow() const{
        return low_;
    }

    const BDD_ID UniqueTableEntry::getTopVar() const{
        return top_var_;
    }

    void UniqueTableEntry::setIsVar() {
        this->is_var_ = true;
    }

    void UniqueTableEntry::setIsConst() {
        this->is_const_ = true;
    }

    bool UniqueTableEntry::getIsVar() const {
        return this->is_var_;
    }

    bool UniqueTableEntry::getIsConst() const {
        return this->is_const_;
    }

    void UniqueTableEntry::setTriple() {
        triple_.clear();
        triple_.push_back(high_);
        triple_.push_back(low_);
        triple_.push_back(top_var_);
    }

    std::vector<BDD_ID> UniqueTableEntry::getTriple() {
        return triple_;
    }

}