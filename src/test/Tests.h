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
    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    //! First: Test the size of the unique Table

    manager->getUniqueTable()->printTable();
    EXPECT_EQ(manager->uniqueTableSize(), 5); //! Size should be 5 with 3 variables and 2 constant values
    EXPECT_EQ(manager->isVariable(a), true); //! All variables should be marked as variable
    EXPECT_EQ(manager->isVariable(b), true);
    EXPECT_EQ(manager->isVariable(c), true);
    EXPECT_EQ(manager->isConstant(c), false); //! Not as constant
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

    //! Third: Test with two Variables
    ClassProject::BDD_ID ID = manager->and2(2,3);
    auto entry = manager->getUniqueTable()->getEntry(ID);
    EXPECT_EQ(entry->getTopVar(), 2 ); //Topvar of 2,3 is expected to be 2
    EXPECT_EQ(entry->getLow(), 0 ); //Low successor is false
    EXPECT_EQ(entry->getHigh(), 3 ); //High successor is expected to be the other variable

    ID = manager->and2(4,5); // c*d
    entry = manager->getUniqueTable()->getEntry(ID);
    EXPECT_EQ(entry->getTopVar(), 4 ); //Topvar of 4,5 is expected to be 4
    EXPECT_EQ(entry->getLow(), 0 ); //Low successor is false
    EXPECT_EQ(entry->getHigh(), 5 ); //High successor is expected to be the other variable

    auto ID2 = manager->and2(6,7); // ((a*b)*(c*d))
    auto entry2 = manager->getUniqueTable()->getEntry(ID2);
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
    EXPECT_EQ(manager->or2(1,1), 1 ); //1+1 returns 1
    EXPECT_EQ(manager->or2(0,0), 0 ); //or with two 0 returns 0
    EXPECT_EQ(manager->or2(0,1), 1 ); //or with one 1 returns 1
    EXPECT_EQ(manager->or2(1,0), 1 ); //or with one 1 returns 1
    //! First: Test Variable with terminal case
    EXPECT_EQ(manager->or2(2,1), 1 ); //or with true returns true
    EXPECT_EQ(manager->or2(3,0), 3 ); //or with false returns the variable
    //! Second: Test the other way round
    EXPECT_EQ(manager->or2(1,2), 1 ); //or with true returns true
    EXPECT_EQ(manager->or2(0,3), 3 ); //or with false returns the variable

    //! Third: Test with two Variables
    ClassProject::BDD_ID ID = manager->or2(2,3); // a+b
    auto entry = manager->getUniqueTable()->getEntry(ID);
    EXPECT_EQ(entry->getTopVar(), 2 ); //Topvar of 2,3 is expected to be 2
    EXPECT_EQ(entry->getLow(), 3 ); //Low successor is expected to be the other variable
    EXPECT_EQ(entry->getHigh(), 1 ); //High successor is expected to be true

    ID = manager->or2(4,5); // c+d
    entry = manager->getUniqueTable()->getEntry(ID);
    EXPECT_EQ(entry->getTopVar(), 4 ); //Topvar of 4,5 is expected to be 4
    EXPECT_EQ(entry->getLow(), 5 ); //Low successor is expected to be the other variable
    EXPECT_EQ(entry->getHigh(), 1 ); //High successor is expected to be true

    auto ID2 = manager->or2(6,7); // ((a+b)+(c+d)
    auto entry2 = manager->getUniqueTable()->getEntry(ID2);
    EXPECT_EQ(entry2->getTopVar(), 2 ); //Topvar of 2,3,4,5 is expected to be 2
    EXPECT_EQ(entry2->getHigh(), 1 );
    EXPECT_EQ(entry2->getLow(), ID+1 );    //Low successor is expected to be ite(b,1,7),
                                                  //which will be appended to the table
                                                  //at ID+1 (after the (c+d))
}

TEST_F(ManagerTest, xor2) {
    /** Check for correct variable creation in the unique table
     */
    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    ClassProject::BDD_ID d = manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->xor2(1,1), 0 ); //xor with two 1 returns 0
    EXPECT_EQ(manager->xor2(0,0), 0 ); //xor with two 0 returns 0
    EXPECT_EQ(manager->xor2(0,1), 1 ); //xor with false and true returns true
    EXPECT_EQ(manager->xor2(1,0), 1 ); //xor with true and false returns true
    //! First: Test Variable with terminal case
    EXPECT_EQ(manager->xor2(a,1), manager->neg(a) ); //xor with true returns the negated variable
    EXPECT_EQ(manager->xor2(a,0), a ); //xor with false returns the variable
    //! Second: Test the other way round
    EXPECT_EQ(manager->xor2(1,a), manager->neg(a) ); //xor with true returns the negated variable
    EXPECT_EQ(manager->xor2(0,a), a ); //xor with false returns the variable

    //! Third: Test with two Variables
    ClassProject::BDD_ID a_xor_b = manager->xor2(a,b); // a xor b
    auto entry = manager->getUniqueTable()->getEntry(a_xor_b);
    EXPECT_EQ(entry->getTopVar(), a ); //Topvar of 2,3 is expected to be 2
    EXPECT_EQ(entry->getLow(), b ); //Low successor is expected to be the other variable
    EXPECT_EQ(entry->getHigh(), manager->neg(b) ); //High successor is expected to be
                                                             //the negated other variable

    ClassProject::BDD_ID c_xor_d = manager->xor2(c,d); // c xor d
    entry = manager->getUniqueTable()->getEntry(c_xor_d);
    EXPECT_EQ(entry->getTopVar(), c ); //Topvar of 4,5 is expected to be 4
    EXPECT_EQ(entry->getLow(), d ); //Low successor is expected to be the other variable
    EXPECT_EQ(entry->getHigh(), manager->neg(d) ); //High successor is expected to be
                                                             //the negated other variable

    auto ID2 = manager->xor2(a_xor_b,c_xor_d); // ((a xor b) xor (c xor d)
    auto entry2 = manager->getUniqueTable()->getTable().at(ID2);
    // manager->getUniqueTable()->printTable();
    EXPECT_EQ(entry2->getTopVar(), a ); //Topvar of 2,3,4,5 is expected to be 2
//    EXPECT_EQ(entry2->getHigh(), c_xor_d ); //ite(!b,!c_xor_d,c_xor_d)
//    EXPECT_EQ(entry2->getLow(), c_xor_d+1 );    //ite(b,!c_xor_d,c_xor_d)


}

TEST_F(ManagerTest, neg) {
    /** Check for correct variable creation in the unique table
     */
    ClassProject::BDD_ID a = manager->createVar("a");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->neg(1), 0 );
    EXPECT_EQ(manager->neg(0), 1 );
    //! First: Test with Variable
    auto entry = manager->getUniqueTable()->getEntry(a);
    auto entry_neg = manager->getUniqueTable()->getEntry(manager->neg(a));
    EXPECT_EQ(entry->getTopVar(), a );
    EXPECT_EQ(entry->getLow(), entry_neg->getHigh() );
    EXPECT_EQ(entry->getHigh(), entry_neg->getLow() );


}

TEST_F(ManagerTest, nand2) {
    /** Check for correct variable creation in the unique table
     */
    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    ClassProject::BDD_ID d = manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->nand2(1,1), 0 );
    EXPECT_EQ(manager->nand2(0,0), 1 );
    EXPECT_EQ(manager->nand2(0,1), 1 );
    EXPECT_EQ(manager->nand2(1,0), 1 );
    //! First: Test Variable with terminal case
    EXPECT_EQ(manager->nand2(a,1), manager->neg(a) ); //nand with true returns the negated variable
    EXPECT_EQ(manager->nand2(a,0), 1 ); //nand with false returns true
    //! Second: Test the other way round
    EXPECT_EQ(manager->nand2(1,b), manager->neg(b) ); //nand with true returns the negated variable
    EXPECT_EQ(manager->nand2(0,b), 1 ); //nand with false returns true

    //! Third: Test with two Variables
    ClassProject::BDD_ID a_nand_b = manager->nand2(a,b);
    auto entry = manager->getUniqueTable()->getEntry(a_nand_b);
    EXPECT_EQ(entry->getTopVar(), a ); //Topvar of 2,3 is expected to be 2
    EXPECT_EQ(entry->getLow(), 1 ); //Low successor with false returns true
    EXPECT_EQ(entry->getHigh(), manager->neg(b) ); //High successor with true returns the negated variable

    ClassProject::BDD_ID c_nand_d = manager->nand2(c,d); // c*d
    entry = manager->getUniqueTable()->getEntry(c_nand_d);
    EXPECT_EQ(entry->getTopVar(), c ); //Topvar of 4,5 is expected to be 4
    EXPECT_EQ(entry->getLow(), 1 ); //Low successor with false returns true
    EXPECT_EQ(entry->getHigh(), manager->neg(d) ); //High successor with true returns the negated variable

    auto nanded_nands = manager->nand2(a_nand_b,c_nand_d); // ((a*b)*(c*d))
    entry = manager->getUniqueTable()->getEntry(nanded_nands);
    EXPECT_EQ(entry->getTopVar(), a ); //Topvar of a,b,c,d is expected to be a
    //EXPECT_EQ(entry->getLow(), 0 ); //Low successor is false

}


TEST_F(ManagerTest, nor2) {
    /** Check for correct variable creation in the unique table
     */
    ClassProject::BDD_ID a = manager->createVar("a");
    ClassProject::BDD_ID b = manager->createVar("b");
    ClassProject::BDD_ID c = manager->createVar("c");
    ClassProject::BDD_ID d = manager->createVar("d");
    //! First: Test the terminal cases
    EXPECT_EQ(manager->nor2(1,1), 0 );
    EXPECT_EQ(manager->nor2(0,0), 1 );
    EXPECT_EQ(manager->nor2(0,1), 0 );
    EXPECT_EQ(manager->nor2(1,0), 0 );
    //! First: Test Variable with terminal case
    EXPECT_EQ(manager->nor2(a,1), 0); //nor with true returns false
    EXPECT_EQ(manager->nor2(a,0), manager->neg(a) ); //nor with false returns the negated variable
    //! Second: Test the other way round
    EXPECT_EQ(manager->nor2(1,b), 0 ); //nor with true returns false
    EXPECT_EQ(manager->nor2(0,b), manager->neg(b) ); //nor with false returns the negated variable

    //! Third: Test with two Variables
    ClassProject::BDD_ID a_nor_b = manager->nor2(a,b);
    auto entry = manager->getUniqueTable()->getEntry(a_nor_b);
    EXPECT_EQ(entry->getTopVar(), a ); //Topvar of 2,3 is expected to be 2
    EXPECT_EQ(entry->getLow(), manager->neg(b) ); //Low successor with false returns the negated variable
    EXPECT_EQ(entry->getHigh(), 0 ); //High successor with true returns false

    ClassProject::BDD_ID c_nor_d = manager->nor2(c,d); // c*d
    entry = manager->getUniqueTable()->getEntry(c_nor_d);
    EXPECT_EQ(entry->getTopVar(), c ); //Topvar of 4,5 is expected to be 4
    EXPECT_EQ(entry->getLow(), manager->neg(d)  ); //Low successor with false returns the negated variable
    EXPECT_EQ(entry->getHigh(), 0); //High successor with true returns false

    auto nored_nors = manager->nand2(a_nor_b,c_nor_d); // ((a*b)*(c*d))
    entry = manager->getUniqueTable()->getEntry(nored_nors);
    EXPECT_EQ(entry->getTopVar(), a ); //Topvar of a,b,c,d is expected to be a
    //EXPECT_EQ(entry->getLow(), 0 ); //Low successor is false
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

TEST_F(ManagerTest, findVars) {
    /* Check findNodes()*/
    //Create sets for vars and expected vars
    std::set<ClassProject::BDD_ID> vars;
    std::set<ClassProject::BDD_ID> x_vars;
    //Trivial Tests
    //! 0 is const and not variable
    x_vars = {};
    manager->findVars(0,vars);
    EXPECT_EQ(vars,x_vars);
    vars.clear();
    //! 1 is const and not variable
    x_vars = {};
    manager->findVars(1,vars);
    EXPECT_EQ(vars,x_vars);
    vars.clear();
    //Test single variable
    //! find variable
    manager->createVar("a");
    x_vars = {2};
    manager->findVars(2,vars);
    EXPECT_EQ(vars,x_vars);
    vars.clear();
    //Test BDD example
    //! 𝑓=(𝑎+𝑏)*𝑐*𝑑

    manager->createVar("b");
    manager->createVar("c");
    manager->createVar("d");

    manager->or2(2,3);
    manager->and2(5,4);
    manager->and2(6,7);

    x_vars = {2,3,4,5};
    manager->findVars(9,vars);
    EXPECT_EQ(vars,x_vars);
    vars.clear();

    //Test some formula that optimizes two variables to one a*b + a*!b
    ClassProject::Manager* manager2 = new ClassProject::Manager();
    manager2->createVar("a");
    manager2->createVar("b");
    manager2->neg(3);
    manager2->and2(2,3);
    manager2->and2(2,4);
    //manager2->getUniqueTable()->printTable();
    x_vars = {2};
    manager2->findVars(manager2->or2(5,6),vars);
    EXPECT_EQ(vars,x_vars);
    vars.clear();



}



#endif //VDS_PROJECT_TESTS_H

