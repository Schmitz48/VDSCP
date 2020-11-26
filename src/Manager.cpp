#include <cassert>

#include "Manager.h"

// Local Variables:
// mode: c++
// End:

namespace ClassProject {



        BDD_ID Manager::createVar(const std::string &label) {
            return 0;
        }

        const BDD_ID &Manager::True() {
            return 0;
        }

        const BDD_ID &Manager::False() {
            return 0;
        }

        bool Manager::isConstant(const BDD_ID f) {
            return false;
        }

        bool Manager::isVariable(const BDD_ID x) {
            return false;
        }

        BDD_ID Manager::topVar(const BDD_ID f) {
            return 0;
        }

        BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e) {
            return 0;
        }

        BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
            return 0;
        }

        BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x) {
            return 0;
        }

        BDD_ID Manager::coFactorTrue(const BDD_ID f) {
            return 0;
        }

        BDD_ID Manager::coFactorFalse(const BDD_ID f) {
            return 0;
        }

        BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b) {
            return 0;
        }

        BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b) {
            return 0;
        }

        BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b) {
            return 0;
        }

        BDD_ID Manager::neg(const BDD_ID a) {
            return 0;
        }

        BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b) {
            return 0;
        }

        BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b) {
            return 0;
        }

        std::string Manager::getTopVarName(const BDD_ID &root) {
            return "";
        }

        void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {

        }

        void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {

        }

        size_t Manager::uniqueTableSize() {
            return 0;
        }



}


    
