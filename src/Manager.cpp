#include <cassert>

#include "Manager.h"

// Local Variables:
// mode: c++
// End:

namespace ClassProject {

        Manager::Manager() {

            auto entry = new UniqueTableEntry(0, "0", 0, 0, 0);
            uniqueTable.insert(std::pair<int, UniqueTableEntry*>(0, entry));
            triple_table.insert(std::pair<std::vector<BDD_ID>, BDD_ID>({0,0,0}, 0));
            entry->setIsConst();
            entry = new UniqueTableEntry(1, "1", 1,1,1);
            entry->setIsConst();
            uniqueTable.insert(std::pair<int, UniqueTableEntry*>(1, entry));
            triple_table.insert(std::pair<std::vector<BDD_ID>, BDD_ID>({1,1,1}, 1));
        }

        BDD_ID Manager::createVar(const std::string &label) {
            BDD_ID id = uniqueTableSize();
            auto entry = new UniqueTableEntry( id, label,1,0,id);
            entry->setIsVar();
            uniqueTable.insert(std::pair<int, UniqueTableEntry*>(id, entry));
            triple_table.insert(std::pair<std::vector<BDD_ID>, BDD_ID>({id,1,0}, id));
            return id;
        }

        const BDD_ID &Manager::True() {
            return uniqueTable.find(1)->second->getID();
        }

        const BDD_ID &Manager::False() {
            return uniqueTable.find(0)->second->getID();
        }

        bool Manager::isConstant(const BDD_ID f) {
            return uniqueTable.find(f)->second->getIsConst();
        }

        bool Manager::isVariable(const BDD_ID x) {
            return uniqueTable.find(x)->second->getIsVar();
        }

        BDD_ID Manager::topVar(const BDD_ID f) {
            return uniqueTable.find(f)->second->getTopVar();
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
            //! Negation
            } else if (t == 0 && e == 1 && false) {

                //! computed table has entry
                std::vector<BDD_ID> ctTriple = {i,t,e};
                auto ct_entry = computed_table.find(ctTriple);
                if (ct_entry != computed_table.end()) {
                    return ct_entry->second;
                }
                //There has to be an entry if we want the entry negated
                auto current = uniqueTable.find(i)->second;
                if (current->getID() == 0) {
                    return 1;
                } else if(current->getID() == 1) {
                    return 0;
                }

                BDD_ID new_low;
                BDD_ID new_high;

                if(current->getLow() == 0) {
                    new_low = 1;
                } else if(current->getLow() == 1) {
                    new_low = 0;
                } else {
                    new_low = neg(current->getLow());
                }

                if(current->getHigh() == 0) {
                    new_high = 1;
                } else if(current->getHigh() == 1) {
                    new_high = 0;
                } else {
                    new_high = neg(current->getHigh());
                }

                //! unique table has entry
                std::vector<BDD_ID> negatedTriple = {current->getTopVar(),new_high,new_low};
                auto ut_entry = triple_table.find(negatedTriple);
                if (ut_entry != triple_table.end()) {
                    return ut_entry->second;
                }
                BDD_ID id = uniqueTableSize();
                auto entry = new UniqueTableEntry(id, "neg", new_high, new_low, current->getTopVar());
                uniqueTable.insert(std::pair<int, UniqueTableEntry*>(id, entry));
                triple_table.insert(std::pair<std::vector<BDD_ID>, BDD_ID>(negatedTriple, id));
                computed_table.insert(std::pair<std::vector<BDD_ID>, BDD_ID>({i,t,e}, id));
                return id;
            }


            //! computed table has entry
            std::vector<BDD_ID> ctTriple = {i,t,e};
            auto ct_entry = computed_table.find(ctTriple);
            if (ct_entry != computed_table.end()) {
                return ct_entry->second;
            }

            //! Find the top variables
            BDD_ID top_var = uniqueTableSize();
            for (const auto& ite: ctTriple) {
                BDD_ID ite_top = uniqueTable.find(ite)->second->getTopVar();
                if (!uniqueTable.find(ite)->second->getIsConst()) {
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


            std::vector<BDD_ID> currentTriple = {top_var,high,low};
            auto ut_entry = triple_table.find(currentTriple);
            if (ut_entry != triple_table.end()) {
                return ut_entry->second;
            }

            BDD_ID id = uniqueTableSize();
            auto entry = new UniqueTableEntry(id, currentNode, high, low, top_var);
            uniqueTable.insert(std::pair<int, UniqueTableEntry*>(id, entry));
            //!add entry to tables
            triple_table.insert(std::make_pair(currentTriple, id));

            computed_table.insert(std::make_pair(ctTriple, id));
            return id;
        }

        BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
            auto f_entry = uniqueTable.find(f)->second;
            auto x_entry = uniqueTable.find(x)->second;
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
            auto f_entry = uniqueTable.find(f)->second;
            auto x_entry = uniqueTable.find(x)->second;
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
        //Cofactor for Top variable
        BDD_ID Manager::coFactorTrue(const BDD_ID f) {
            return coFactorTrue(f, topVar(f));
        }
        //Cofactor for Top variable
        BDD_ID Manager::coFactorFalse(const BDD_ID f) {
            return coFactorFalse(f, topVar(f));
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
            //! xor = ite(a,0,1)
            currentNode = "not";
            return ite(a,0,1);
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
            return uniqueTable.find(topVar(root))->second->getLabel();
        }

        void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
            std::vector<BDD_ID> queue;
            queue.push_back(root);
            nodes_of_root.insert(root);
            while(!queue.empty()) {
                auto queue_entry = uniqueTable.find(queue.front())->second;
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
                auto queue_entry = uniqueTable.find(queue.front())->second;
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
            return uniqueTable.size();
        }

        std::unordered_map<int, UniqueTableEntry*> Manager::getUniqueTable() {
            return this->uniqueTable;
        }

}


    
