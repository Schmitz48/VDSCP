//
// Created by ludwig on 04.12.18.
//

#ifndef PROJECT_REACHABLEINTERFACE_H
#define PROJECT_REACHABLEINTERFACE_H

#include "../Manager.h"

namespace ClassProject{
    class ReachableInterface: public Manager{
    public:
        ReachableInterface() = delete;

        /**
         * Constructor creates stateSize state bits for the user
         * @param stateSize state size
         */
        explicit ReachableInterface(unsigned int stateSize){};
        virtual ~ReachableInterface() = default;

         //! @return returns the XNOR of BDD IDs
        virtual BDD_ID xnor2(BDD_ID a, BDD_ID b) = 0;
        /**
         * States are generated and stored in a vector.
         * These lsb (e.g. "s0")  is stored at location 0.
         * The msb(e.g. "s3") is stored at location size-1.
         * @return vector with the BDD_ID of each state bit
         */
        virtual const std::vector<BDD_ID> &getStates() const = 0 ;

        /**
         * Each state variable has a transition function.
         * The transition function specifies the value of the state after the transition.
         * The transition function are composed of only state variables.
         * For example: s0' = s0 XOR s1
         * The next state is defined as XOR of the current values of the state bit s0 and s1
         * @param transitionFunctions provide a transition function exactly for each state bit
         */
        virtual void setDelta(const std::vector<BDD_ID> &transitionFunctions)  = 0;

        /**
         * Each state machine has an inital state. The inital state is provided as a vector.
         * The vector has to have an entry for each state bit. If the entry is "true" the state bit is high,
         * otherwhise negated. E.g. initial state not(s0) and not(s1) is transformed into {false,false}.
         * @param stateVector provide the assignemtn for each state bit
         */
        virtual void setInitState(const std::vector<bool>& stateVector)  = 0;

        /**
         * Computes the symbolic representation of the reachable states.
         * Before this method is called it is important to set the transition function and the initial state.
         * @return BDD_ID of the reachable state space
         */
        virtual BDD_ID compute_reachable_states() = 0;

        /**
         * This method decides whether a specific state is in the reachable state space or not.
         * The inpute is provided as a vector of states. The value is true if the state bit is high in this state, false otherwise.
         * @param stateVector 
         * @return
         */
        virtual bool is_reachable(const std::vector<bool>& stateVector) = 0;
    private:
        unsigned int stateSize;
    };
}


#endif //PROJECT_REACHABLEINTERFACE_H
