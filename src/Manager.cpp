#include <cassert>

#include "Manager.h"

// Local Variables:
// mode: c++
// End:

namespace ClassProject {

        Manager::Manager() {
            uniqueTable = new UniqueTable();
        }

        BDD_ID Manager::createVar(const std::string &label) {
            auto entry = new UniqueTableEntry(uniqueTableSize(), label,1,0,uniqueTableSize());
            entry->setIsVar();
            uniqueTable->insertEntry(entry);
            return uniqueTableSize();
        }

        const BDD_ID &Manager::True() {
            return uniqueTable->getEntry(1)->getID();
        }

        const BDD_ID &Manager::False() {
            return uniqueTable->getEntry(0)->getID();
        }

        bool Manager::isConstant(const BDD_ID f) {
            return uniqueTable->getEntry(f)->getIsConst();
        }

        bool Manager::isVariable(const BDD_ID x) {
            return uniqueTable->getEntry(x)->getIsVar();
        }

        BDD_ID Manager::topVar(const BDD_ID f) {
            return uniqueTable->getEntry(f)->getTopVar();
        }

        BDD_ID Manager::ite(const BDD_ID i, const BDD_ID t, const BDD_ID e) {
            //!Check for terminal cases
            if(i == 1) {
                return t;
            } else if(i == 0) {
                return e;
            } else if (t == 1 && e == 0) {
                return i;
            } else if (t == e) {
                return e;
            } else if (t == 0 && e == 1) {
                return neg(i);
            }
            std::set<BDD_ID> ids {i,t,e};
            BDD_ID top_var = uniqueTableSize();
            //! Find the top variables
            for (const auto& ite: ids) {
                BDD_ID ite_top = uniqueTable->getEntry(ite)->getTopVar();
                if (!uniqueTable->getEntry(ite_top)->getIsConst()) {
                    if(top_var > ite_top) {
                        top_var = ite_top;
                    }
                }
            }
            //! Find high and low successor
            BDD_ID i_true = coFactorTrue(i,top_var );
            BDD_ID t_true = coFactorTrue(t,top_var );
            BDD_ID e_true = coFactorTrue(e,top_var);
            BDD_ID i_false = coFactorFalse(i,top_var );
            BDD_ID t_false = coFactorFalse(t,top_var );
            BDD_ID e_false = coFactorFalse(e,top_var);
            BDD_ID high = ite(i_true, t_true, e_true );
            BDD_ID low = ite(i_false,t_false, e_false);
            if (high == low) {
                return high;
            }
            BDD_ID id = uniqueTableSize();
            auto entry = new UniqueTableEntry(id, currentNode, high, low, top_var);
            std::set<BDD_ID> currentTriple {high,low,top_var};
            for (const auto & table: uniqueTable->getTable()) {
                if (table->getTriple() == currentTriple) {
                    return table->getID();
                }
            }
            uniqueTable->insertEntry(entry);
            /*let x be the top-variable of (f, g, h);
            rhigh:= ite(f|x=1, g|x=1, h|x=1);
            rlow:= ite(f|x=0, g|x=0, h|x=0);
            if(rhigh== rlow)  reduction is possible
                return rhigh;
            r:= find_or_add_unique_table(x, rlow, rhigh); /* eliminate isomorphic sub-graphs  update_computed_table((f, g, h), r);
            returnr;*/
            //coFactorTrue();
            return id;
        }

        BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
            auto f_entry = uniqueTable->getEntry(f);
            auto x_entry = uniqueTable->getEntry(x);
            bool terminal = f_entry->getIsConst() || x_entry->getIsConst() || f_entry->getTopVar() > x;
            if(terminal) {
                return f;
            }
            if(f_entry->getTopVar() == x) {
                return f_entry->getHigh();
            } else {
                BDD_ID T = coFactorTrue(f_entry->getHigh(), x);
                BDD_ID F = coFactorTrue(f_entry->getLow(), x);
                return ite(f_entry->getTopVar(), T, F);
            }
        }

        BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x) {
            auto f_entry = uniqueTable->getEntry(f);
            auto x_entry = uniqueTable->getEntry(x);
            bool terminal = f_entry->getIsConst() || x_entry->getIsConst() || f_entry->getTopVar() > x;
            if(terminal) {
                return f;
            }
            if(f_entry->getTopVar() == x) {
                return f_entry->getLow();
            } else {
                BDD_ID T = coFactorFalse(f_entry->getHigh(), x);
                BDD_ID F = coFactorFalse(f_entry->getLow(), x);
                return ite(f_entry->getTopVar(), T, F);
            }
        }

        BDD_ID Manager::coFactorTrue(const BDD_ID f) {
            return 0;
        }

        BDD_ID Manager::coFactorFalse(const BDD_ID f) {
            return 0;
        }

        BDD_ID Manager::and2(const BDD_ID a, const BDD_ID b) {
            currentNode = "and";
            return ite(a,b,0);
        }

        BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b) {
            currentNode = "or";
            return ite(a,1,b);
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
            return uniqueTable->getTable().size();
        }

        UniqueTable* Manager::getUniqueTable() {
            return this->uniqueTable;
        }

}


    
