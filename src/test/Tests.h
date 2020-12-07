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
    //! First: Test the size of the unique Table
    EXPECT_EQ(manager->and2(2,1), 2 ); //and with true returns the variable
    EXPECT_EQ(manager->and2(3,0), 0 ); //and with false returns the false
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


#endif //VDS_PROJECT_TESTS_H

