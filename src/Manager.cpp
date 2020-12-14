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
            BDD_ID id = uniqueTableSize();
            auto entry = new UniqueTableEntry( id, label,1,0,id);
            entry->setIsVar();
            uniqueTable->insertEntry(entry);
            return id;
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
            std::vector<BDD_ID> currentTriple = {high,low,top_var};
            for (const auto & table: uniqueTable->getTable()) {
                if (table->getTriple() == currentTriple) {
                    return table->getID();
                }
            }
            uniqueTable->insertEntry(entry);
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
            //! and = ite(a,b,0)
            currentNode = "and";
            return ite(a,b,0);
        }

        BDD_ID Manager::or2(const BDD_ID a, const BDD_ID b) {
            //! or = ite(a,1,b)
            currentNode = "or";
            return ite(a,1,b);
        }

        BDD_ID Manager::xor2(const BDD_ID a, const BDD_ID b) {
            //! xor = ite(a,neg(b),b)
            currentNode = "xor";
            return ite(a,neg(b),b);
        }

        BDD_ID Manager::neg(const BDD_ID a) {
            //! For trivial cases: return the opposite

            //! For non trivial cases: switch high and low successor and find or add to table
            BDD_ID id = uniqueTableSize();
            auto current = uniqueTable->getEntry(a);
            if (current->getID() == 0) {
                return 1;
            } else if(current->getID() == 1) {
                return 0;
            }
            auto entry = new UniqueTableEntry(id, "neg", current->getLow(), current->getHigh(), current->getTopVar());
            std::vector<BDD_ID> currentTriple = {current->getLow(),current->getHigh(),current->getTopVar()};
            for (const auto & table: uniqueTable->getTable()) {
                if (table->getTriple() == currentTriple) {
                    return table->getID();
                }
            }
            uniqueTable->insertEntry(entry);
            return id;
        }

        BDD_ID Manager::nand2(const BDD_ID a, const BDD_ID b) {
            //! nand = ite(a,neg(b),1)
            currentNode = "nand";
            return ite(a,neg(b),1);
        }

        BDD_ID Manager::nor2(const BDD_ID a, const BDD_ID b) {
            //! nor = ite(a,0,neg(b))
            currentNode = "nor";
            return ite(a,0,neg(b));
        }

        std::string Manager::getTopVarName(const BDD_ID &root) {
            return uniqueTable->getEntry(root)->getLabel();
        }

        void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
            std::vector<BDD_ID> queue;
            queue.push_back(root);
            nodes_of_root.insert(root);
            while(!queue.empty()) {
                auto queue_entry = uniqueTable->getEntry(queue.front());
                if(!queue_entry->getIsConst()) {
                    queue.push_back(queue_entry->getHigh());
                    queue.push_back(queue_entry->getLow());
                }
                nodes_of_root.insert(queue_entry->getID());
                queue.erase(queue.begin());
            }

        }

        void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {

            std::vector<BDD_ID> queue;
            if (isConstant(root)) {
                return;
            }
            queue.push_back(root);
            while(!queue.empty()) {
                auto queue_entry = uniqueTable->getEntry(queue.front());
                if(!queue_entry->getIsConst()) {
                    queue.push_back(queue_entry->getHigh());
                    queue.push_back(queue_entry->getLow());
                }
                if(!isConstant(queue_entry->getTopVar())) {
                    vars_of_root.insert(queue_entry->getTopVar());
                }
                queue.erase(queue.begin());
            }
        }

        size_t Manager::uniqueTableSize() {
            return uniqueTable->getTable().size();
        }

        UniqueTable* Manager::getUniqueTable() {
            return this->uniqueTable;
        }

}


    
