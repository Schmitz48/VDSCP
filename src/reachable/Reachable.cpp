//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"

namespace ClassProject {


    Reachable::Reachable(unsigned int stateSize) : ReachableInterface(stateSize) {
        manager = new Manager();
        for(int i = 0; i < stateSize; i++) {
            stateVector.push_back(i);
            manager->createVar("s" + std::to_string(i));
        }
        for(int i = 0; i < stateSize; i++) {
            manager->createVar("s" + std::to_string(i) + "'");
        }


    }

    BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b) {
        return 0;
    }

    const std::vector<BDD_ID> &Reachable::getStates() const {
        return stateVector;
    }


    void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions)  {

    }

    void Reachable::setInitState(const std::vector<bool>& stateVector) {

    }

    BDD_ID Reachable::compute_reachable_states() {
        return 0;
    }

    bool Reachable::is_reachable(const std::vector<bool>& stateVector){
        return false;
    }
}