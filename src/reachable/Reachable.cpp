//
// Created by ludwig on 27.11.18.
//

#include "Reachable.h"

namespace ClassProject {


    Reachable::Reachable(unsigned int stateSize) : ReachableInterface(stateSize) {
        manager = new Manager();
        for(int i = 0; i < stateSize; i++) {
            stateVector_.push_back(manager->createVar("s" + std::to_string(i)));
        }
        for(int i = 0; i < stateSize; i++) {
            stateVector_.push_back(manager->createVar("s" + std::to_string(i) + "'"));
        }
        this->stateSize = stateSize;

    }

    BDD_ID Reachable::xnor2(BDD_ID a, BDD_ID b) {
        return manager->ite(a,b,manager->neg(b));
    }

    const std::vector<BDD_ID> &Reachable::getStates() const {
        return stateVector_;
    }


    void Reachable::setDelta(const std::vector<BDD_ID> &transitionFunctions)  {
        this->functions = transitionFunctions;
    }

    void Reachable::setInitState(const std::vector<bool>& stateVector) {
        initVector = stateVector;
    }

    BDD_ID Reachable::compute_reachable_states() {
        //compute tau(s,x,s')
        //compute function for initial state
        BDD_ID tau = 1;
        //transition relation tau
        for (int i  = 0; i < stateSize; i++) {
            //tau = manager->and2(tau, manager->or2(manager->and2(stateVector.at(i+stateSize),functions.at(i)),  manager->and2(manager->neg(stateVector.at(i+stateSize)),manager->neg(functions.at(i)))));
            //! seems to work
            tau = manager->and2(tau, xnor2(stateVector_.at(i+stateSize),functions.at(i)));
        }
        //cS0
        BDD_ID cs0 = tau;
        for (int i  = 0; i < stateSize; i++) {
            BDD_ID tmp;
            if (initVector.at(i)) {
                tmp = stateVector_.at(i);
            } else {
                tmp = manager->neg(stateVector_.at(i));
            }
            cs0 = manager->and2(cs0,  tmp);
        }
        //! currently cs0 = tau*startValues for test example : !s0!s1s0's1'
        //! next Existential quantifier
        for (int i  = 0; i < stateSize; i++) {
            cs0 = manager->or2(manager->coFactorTrue(cs0, stateVector_.at(i)),manager->coFactorFalse(cs0, stateVector_.at(i)));
        }
        //! we obtained the new state(s) after the initial state s0's1'
        //! transform this into s0s1

        BDD_ID imageCs0 = 1;
        for (int i  = 0; i < stateSize; i++) {
            imageCs0 = manager->and2(imageCs0, xnor2(stateVector_.at(i), stateVector_.at(i+stateSize)));
        }
        imageCs0 = manager->and2(imageCs0, cs0);
        for(int i = 0; i < stateSize; i++) {
            imageCs0 = manager->or2(manager->coFactorTrue(imageCs0, stateVector_.at(i + stateSize)), manager->coFactorFalse(imageCs0, stateVector_.at(i + stateSize)));
        }

        BDD_ID cRit = imageCs0;
        BDD_ID cR;
        //! Everything works as expected until here
        do {
            cR = cRit;
            //compute image
            BDD_ID image_prime = manager->and2(cR, tau);
            for(int i = 0; i < stateSize; i++) {
                image_prime = manager->or2(manager->coFactorTrue(image_prime, stateVector_.at(i)), manager->coFactorFalse(image_prime, stateVector_.at(i)));
            }

            BDD_ID image = 1;
            for (int i  = 0; i < stateSize; i++) {
                image = manager->and2(image, xnor2(stateVector_.at(i), stateVector_.at(i+stateSize)));
            }
            image = manager->and2(image, image_prime);
            for(int i = 0; i < stateSize; i++) {
                image = manager->or2(manager->coFactorTrue(image, stateVector_.at(i + stateSize)), manager->coFactorFalse(image, stateVector_.at(i + stateSize)));
            }
            cRit = manager->or2(cR, image);

        } while (cRit != cR); //


        return cR;
    }

    bool Reachable::is_reachable(const std::vector<bool>& stateVector){
        BDD_ID reachable = compute_reachable_states();
        for(int i = 0; i < stateVector.size(); i++) {
            if(stateVector.at(i)) {
                reachable = manager->coFactorTrue(reachable,  stateVector_.at(i));
            } else {
                reachable = manager->coFactorFalse(reachable,  stateVector_.at(i));
            }
        }
        if(reachable == 1) {
            return true;
        } else {
            return false;
        }

    }
}