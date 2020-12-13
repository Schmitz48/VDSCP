//
// Created by tobias on 21.11.16.
//

#ifndef VDS_PROJECT_GTESTS_H
#define VDS_PROJECT_GTESTS_H

#include "gtest/gtest.h"
#include "../Manager.h"

class ManagerTest : public ::testing::Test {
public:
    /** Contains tests for the Manager class
     * a new manager object is constructed with the initialization
    */
    ClassProject::Manager* manager;
    ManagerTest(): manager(new ClassProject::Manager()){}
};

TEST_F(ManagerTest, initialization) {
    /** Check for correct initialization of the unique table
     */
    EXPECT_EQ(manager->False(), 0); //! False should be 0 after initialization
    EXPECT_EQ(manager->True(), 1); //! True should be 1 after initialization
}

TEST_F(ManagerTest, createVar) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    //! First: Test the size of the unique Table
    EXPECT_EQ(manager->uniqueTableSize(), 5); //! Size should be 5 with 3 variables and 2 constant values
    EXPECT_EQ(manager->isVariable(2), true); //! All variables should be marked as variable
    EXPECT_EQ(manager->isVariable(3), true);
    EXPECT_EQ(manager->isVariable(4), true);
    EXPECT_EQ(manager->isConstant(4), false); //! Not as constant
    //TODO use: void findVars(const BDD_ID &root, std::set<BDD_ID> &vars_of_root) ?


}

TEST_F(ManagerTest, and2) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->and2(1,1), 1 ); //1*1 returns 1
    EXPECT_EQ(manager->and2(0,0), 0 ); //and with one 0 returns 0
    EXPECT_EQ(manager->and2(0,1), 0 ); //and with false returns the false
    EXPECT_EQ(manager->and2(1,0), 0 ); //and with false returns the false
    //! First: Test Variable with terminal case
    EXPECT_EQ(manager->and2(2,1), 2 ); //and with true returns the variable
    EXPECT_EQ(manager->and2(3,0), 0 ); //and with false returns the false
    //! Second: Test the other way round
    EXPECT_EQ(manager->and2(1,2), 2 ); //and with true returns the variable
    EXPECT_EQ(manager->and2(0,3), 0 ); //and with false returns the false

    ClassProject::BDD_ID ID = manager->and2(2,3);
    auto entry = manager->getUniqueTable()->getTable().at(ID);
    EXPECT_EQ(entry->getTopVar(), 2 ); //Topvar of 2,3 is expected to be 2
    EXPECT_EQ(entry->getLow(), 0 ); //Low successor is false
    EXPECT_EQ(entry->getHigh(), 3 ); //High successor is expected to be the other variable

    ID = manager->and2(4,5); // c*d
    entry = manager->getUniqueTable()->getTable().at(ID);
    EXPECT_EQ(entry->getTopVar(), 4 ); //Topvar of 4,5 is expected to be 4
    EXPECT_EQ(entry->getLow(), 0 ); //Low successor is false
    EXPECT_EQ(entry->getHigh(), 5 ); //High successor is expected to be the other variable


    auto ID2 = manager->and2(6,7); // ((a*b)*c)
    auto entry2 = manager->getUniqueTable()->getTable().at(ID2);
    EXPECT_EQ(entry2->getTopVar(), 2 ); //Topvar of 2,3,4,5 is expected to be 2
    EXPECT_EQ(entry2->getLow(), 0 ); //Low successor is false



}

TEST_F(ManagerTest, or2) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->or2(1,1), 1 ); //1*1 returns 1
    EXPECT_EQ(manager->or2(0,0), 0 ); //and with one 0 returns 0
    EXPECT_EQ(manager->or2(0,1), 1 ); //and with false returns the false
    EXPECT_EQ(manager->or2(1,0), 1 ); //and with false returns the false
    //! First: Test Variable with terminal case
    EXPECT_EQ(manager->or2(2,1), 1 ); //and with true returns the variable
    EXPECT_EQ(manager->or2(3,0), 3 ); //and with false returns the false
    //! Second: Test the other way round
    EXPECT_EQ(manager->or2(1,2), 1 ); //and with true returns the variable
    EXPECT_EQ(manager->or2(0,3), 3 ); //and with false returns the false
}

TEST_F(ManagerTest, xor2) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->xor2(1,1), 0 ); //1*1 returns 1
    EXPECT_EQ(manager->xor2(0,0), 0 ); //and with one 0 returns 0
    EXPECT_EQ(manager->xor2(0,1), 1 ); //and with false returns the false
    EXPECT_EQ(manager->xor2(1,0), 1 ); //and with false returns the false

}

TEST_F(ManagerTest, neg) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->neg(1), 0 ); //1*1 returns 1
    EXPECT_EQ(manager->neg(0), 1 ); //and with one 0 returns 0

}

TEST_F(ManagerTest, nand2) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->nand2(1,1), 0 ); //1*1 returns 1
    EXPECT_EQ(manager->nand2(0,0), 1 ); //and with one 0 returns 0
    EXPECT_EQ(manager->nand2(0,1), 1 ); //and with false returns the false
    EXPECT_EQ(manager->nand2(1,0), 1 ); //and with false returns the false

}

TEST_F(ManagerTest, nor2) {
    /** Check for correct variable creation in the unique table
     */
    manager->createVar("a");
    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->nor2(1,1), 0 ); //1*1 returns 1
    EXPECT_EQ(manager->nor2(0,0), 1 ); //and with one 0 returns 0
    EXPECT_EQ(manager->nor2(0,1), 0 ); //and with false returns the false
    EXPECT_EQ(manager->nor2(1,0), 0 ); //and with false returns the false

}

TEST_F(ManagerTest, findNodes) {
    /* Check findNodes()*/
    //Create sets for nodes and expected nodes
    std::set<ClassProject::BDD_ID> nodes;
    std::set<ClassProject::BDD_ID> x_nodes;
    //Trivial Tests
    //! 0 is the only node
    x_nodes = {0};
    manager->findNodes(0,nodes);
    EXPECT_EQ(nodes,x_nodes);
    nodes.clear();
    //! 1 is the only node
    x_nodes = {1};
    manager->findNodes(1,nodes);
    EXPECT_EQ(nodes,x_nodes);
    nodes.clear();
    //Test single variable
    //! variable 1 and 0
    manager->createVar("a");
    x_nodes = {2,1,0};
    manager->findNodes(2,nodes);
    EXPECT_EQ(nodes,x_nodes);
    nodes.clear();
    //Test BDD example
    //𝑓=(𝑎+𝑏).𝑐.𝑑

    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");

    manager->or2(2,3);
    manager->and2(5,4);
    manager->and2(6,7);

    x_nodes = {9,8,7,5,1,0};
    manager->findNodes(9,nodes);
    EXPECT_EQ(nodes,x_nodes);
}



#endif //VDS_PROJECT_TESTS_H

