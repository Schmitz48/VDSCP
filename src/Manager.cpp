#include <cassert>
#include <queue>

#include "Manager.h"

// Local Variables:
// mode: c++
// End:

namespace ClassProject {

        Manager::Manager() {

            auto entry = new UniqueTableEntry(0, "0", 0, 0, 0);
            uniqueTable.insert(std::make_pair(0, entry));
            triple_t zero = {0,0,0};
            triple_table_.insert(robin_hood::pair<triple_t, BDD_ID>(zero, 0));
            entry->setIsConst();
            entry = new UniqueTableEntry(1, "1", 1,1,1);
            entry->setIsConst();
            uniqueTable.insert(std::make_pair(1, entry));
            triple_t one = {1,1,1};
            triple_table_.insert(robin_hood::pair<triple_t, BDD_ID>(one, 1));
        }

        BDD_ID Manager::createVar(const std::string &label) {
            BDD_ID id = uniqueTableSize();
            auto entry = new UniqueTableEntry( id, label,1,0,id);
            entry->setIsVar();
            uniqueTable.insert(std::pair<int, UniqueTableEntry*>(id, entry));
            triple_t var = {id,1,0};
            triple_table_.insert(robin_hood::pair<triple_t, BDD_ID>(var, id));
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
            } else if (t == 0 && e == 1) {

                //! computed table has entry
                triple_t ctTriple = {i,t,e};
                auto ct_entry = computed_table_.find(ctTriple);
                if (ct_entry != computed_table_.end()) {
                    return ct_entry->second;
                }
                //There has to be an entry if we want the entry negated
                auto current = uniqueTable.find(i)->second;
                if (current->id_ == 0) {
                    return 1;
                } else if(current->id_ == 1) {
                    return 0;
                }

                BDD_ID new_low;
                BDD_ID new_high;

                if(current->low_ == 0) {
                    new_low = 1;
                } else if(current->low_ == 1) {
                    new_low = 0;
                } else {
                    new_low = neg(current->low_);
                }

                if(current->high_ == 0) {
                    new_high = 1;
                } else if(current->high_ == 1) {
                    new_high = 0;
                } else {
                    new_high = neg(current->high_);
                }

                //! unique table has entry
                triple_t negatedTriple = {current->top_var_,new_high,new_low};
                auto ut_entry = triple_table_.find(negatedTriple);
                if (ut_entry != triple_table_.end()) {
                    return ut_entry->second;
                }
                BDD_ID id = uniqueTable.size();
                auto entry = new UniqueTableEntry(id, "neg", new_high, new_low, current->top_var_);
                uniqueTable.insert(std::make_pair(id, entry));
                triple_table_.insert(robin_hood::pair<triple_t, BDD_ID>(negatedTriple, id));
                triple_t ite = {i,t,e};
                computed_table_.insert(robin_hood::pair<triple_t, BDD_ID>(ite, id));
                return id;
            }


            //! computed table has entry
            triple_t ctTriple = {i,t,e};
            auto ct_entry = computed_table_.find(ctTriple);
            if (ct_entry != computed_table_.end()) {
                return ct_entry->second;
            }

            //! Find the top variables
            std::vector<BDD_ID> top_var_check = {i,t,e};
            BDD_ID top_var = uniqueTableSize();
            for (const auto& ite: top_var_check) {
                BDD_ID ite_top = uniqueTable.find(ite)->second->top_var_;
                if (!uniqueTable.find(ite)->second->is_const_){
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

            triple_t currentTriple = {top_var,high,low};
            auto ut_entry = triple_table_.find(currentTriple);
            if (ut_entry != triple_table_.end()) {
                return ut_entry->second;
            }

            BDD_ID id = uniqueTable.size();
            auto entry = new UniqueTableEntry(id, currentNode, high, low, top_var);
            uniqueTable.insert(std::make_pair(id, entry));
            //!add entry to tables
            triple_table_.insert(robin_hood::pair<triple_t, BDD_ID>(currentTriple, id));
            computed_table_.insert(robin_hood::pair<triple_t, BDD_ID>(ctTriple, id));
            return id;
        }

        BDD_ID Manager::coFactorTrue(const BDD_ID f, BDD_ID x) {
            auto f_entry = uniqueTable.find(f)->second;
            auto x_entry = uniqueTable.find(x)->second;
            bool terminal = f_entry->is_const_ || x_entry->is_const_ || f_entry->top_var_ > x;
            if(terminal) {
                return f;
            }
            if(f_entry->top_var_ == x) {
                return f_entry->high_;
            } else {
                BDD_ID T = coFactorTrue(f_entry->high_, x);
                BDD_ID F = coFactorTrue(f_entry->low_, x);
                return ite(f_entry->top_var_, T, F);
            }
        }

        BDD_ID Manager::coFactorFalse(const BDD_ID f, BDD_ID x) {
            auto f_entry = uniqueTable.find(f)->second;
            auto x_entry = uniqueTable.find(x)->second;
            bool terminal = f_entry->is_const_ || x_entry->is_const_ || f_entry->top_var_ > x;
            if(terminal) {
                return f;
            }
            if(f_entry->top_var_ == x) {
                return f_entry->low_;
            } else {
                BDD_ID T = coFactorFalse(f_entry->high_, x);
                BDD_ID F = coFactorFalse(f_entry->low_, x);
                return ite(f_entry->top_var_, T, F);
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
            return uniqueTable.find(topVar(root))->second->label_;
        }

        void Manager::findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) {
            std::queue<BDD_ID> queue;
            queue.push(root);
            nodes_of_root.insert(root);
            while(!queue.empty()) {
                auto queue_entry = uniqueTable.find(queue.front())->second;
                if(!queue_entry->is_const_) {
                    queue.push(queue_entry->high_);
                    queue.push(queue_entry->low_);
                }
                nodes_of_root.insert(queue_entry->id_);
                queue.pop();
            }

        }

        void Manager::findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) {

            std::queue<BDD_ID> queue;
            if (isConstant(root)) {
                return;
            }
            queue.push(root);
            while(!queue.empty()) {
                auto queue_entry = uniqueTable.find(queue.front())->second;
                if(!queue_entry->is_const_) {
                    queue.push(queue_entry->high_);
                    queue.push(queue_entry->low_);
                }
                if(!isConstant(queue_entry->top_var_)) {
                    vars_of_root.insert(queue_entry->top_var_);
                }
                queue.pop();
            }
        }

        size_t Manager::uniqueTableSize() {
            return uniqueTable.size();
        }

        std::unordered_map<int, UniqueTableEntry*> Manager::getUniqueTable() {
            return this->uniqueTable;
        }

}


    
