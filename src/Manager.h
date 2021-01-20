// A minimalistic BDD library, following Wolfgang Kunz lecture slides
//
// Written by Markus Wedler 2014

#ifndef mwBDD_H
#define mwBDD_H

#include <cassert>
#include <iostream>
#include <list>
#include <vector>
#include <string>
#include <unordered_map>
#include "ManagerInterface.h"
#include "UniqueTable.h"

namespace ClassProject {


    //!  Manager
    /*!
    The Manager class inherits and implements the pure virtual functions from the ManagerInterface.
    It manages the BDD represented by the private member uniqueTable.
    */
    struct VectorHasher {
        int operator()(const std::vector<BDD_ID> &V) const {
            int hash = V.size();
            for(const auto&i:V) {
                hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
            }
            return hash;
        }

    };

    class Manager: public ManagerInterface{
    public:

        Manager();


        /*! \fn BDD_ID createVar(const std::string &label)
        \brief Adds a new entry for the variable with the label "label" to the unique table
        \param label Label of the new variable
        */
        BDD_ID createVar(const std::string &label) override;

        /*! \fn const BDD_ID &True()
        \brief  Returns the ID of the constant true node
        */
        const BDD_ID &True() override;

        /*! \fn const BDD_ID &False()
        \brief  Returns the ID of the constant false node
        */
        const BDD_ID &False() override;

        /*! \fn bool isConstant(const BDD_ID f)
        \brief Returns true if the node with the BDD ID f is a constant and false otherwise
        \param f ID of the examined node
        */
        bool isConstant(const BDD_ID f) override;

        /*! \fn bool isVariable(const BDD_ID x)
        \brief Returns true if the node with the BDD ID f is a variable and false otherwise
        \param x ID of the examined node
        */
        bool isVariable(const BDD_ID x) override;

        /*! \fn bool isVariable(const BDD_ID x)
        \brief Returns the ID of the top variable of the node with the BDD ID f
        \param f ID of the examined node
        */
        BDD_ID topVar(const BDD_ID f) override;

        /*! \fn BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e)
        \brief Performs an if-then-else operation: If i then t else e
        \param i BDD ID of the if operator
        \param t BDD ID of the then operator
        \param e BDD ID of the else operator
        */
        BDD_ID ite(const BDD_ID i, const BDD_ID t, const BDD_ID e) override;

        /*! \fn BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x)
        \brief Performs a co factor operation for f|x=true
        \param f: BDD ID of the node
        \param x: BDD ID of the variable to be cofactored
        */
        BDD_ID coFactorTrue(const BDD_ID f, BDD_ID x) override;


        /*! \fn BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x)
        \brief Performs a co factor operation for f|x=false
        \param f: BDD ID of the node
        \param x: BDD ID of the variable to be cofactored
        */
        BDD_ID coFactorFalse(const BDD_ID f, BDD_ID x) override;


        BDD_ID coFactorTrue(const BDD_ID f) override;
        //! What should this do? - High or low
        BDD_ID coFactorFalse(const BDD_ID f) override;

        /*! \fn BDD_ID and2(const BDD_ID a, const BDD_ID b)
        \brief Performs (a and b) and returns the BDD ID of the result
        \param a BDD ID of the first operand
        \param b BDD ID of the second operand
        */
        BDD_ID and2(const BDD_ID a, const BDD_ID b) override;

        /*! \fn BDD_ID or2(const BDD_ID a, const BDD_ID b)
        \brief Performs (a or b) and returns the BDD ID of the result
        \param a BDD ID of the first operand
        \param b BDD ID of the second operand
        */
        BDD_ID or2(const BDD_ID a, const BDD_ID b) override;

        /*! \fn BDD_ID xor2(const BDD_ID a, const BDD_ID b)
        \brief Performs (a xor b) and returns the BDD ID of the result
        \param a BDD ID of the first operand
        \param b BDD ID of the second operand
        */
        BDD_ID xor2(const BDD_ID a, const BDD_ID b) override;

        /*! \fn BDD_ID neg(const BDD_ID a)
        \brief Performs not(a) and returns the BDD ID of the result
        \param a BDD ID of the operand
        */
        BDD_ID neg(const BDD_ID a) override;

        /*! \fn BDD_ID nand2 (const BDD_ID a, const BDD_ID b)
        \brief Performs (a nand b) and returns the BDD ID of the result
        \param a BDD ID of the first operand
        \param b BDD ID of the second operand
        */
        BDD_ID nand2(const BDD_ID a, const BDD_ID b) override;

        /*! \fn BDD_ID nor2(const BDD_ID a, const BDD_ID b)
        \brief Performs (a nor b) and returns the BDD ID of the result
        \param a BDD ID of the first operand
        \param b BDD ID of the second operand
        */
        BDD_ID nor2(const BDD_ID a, const BDD_ID b) override;

        /*! \fn std::string getTopVarName(const BDD_ID &root)
        \brief Returns the label of root's top var
        \param root BDD node
        */
        std::string getTopVarName(const BDD_ID &root) override;

        /*! \fn void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root)
        \brief Iterates through the BDD and adds the nodes to the set (in-place computation on nodes_of_root)
        \param root Root node of the BDD to be examined
        \param nodes_of_root set of nodes
        */
        void findNodes(const BDD_ID &root, std::set<BDD_ID> &nodes_of_root) override;

        /*! \fn void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root)
       \brief Iterates through the BDD and adds the top variables of the nodes to the set (in-place computation on vars_of_root)
       \param root Root node of the BDD to be examined
       \param vars_of_root set of nodes
       */
        void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) override;

        /*! \fn size_t uniqueTableSize()
         \brief Returns the current size of the unique table
         */
        size_t uniqueTableSize() override;

        UniqueTable* getUniqueTable();

    private:
        UniqueTable* uniqueTable;   /*!< The unique table representing the BDD*/
        std::string currentNode;
        //std::map<std::vector<BDD_ID>, BDD_ID> computed_table;
        std::unordered_map<std::vector<BDD_ID>, BDD_ID, VectorHasher> computed_table;

    };


}
#endif