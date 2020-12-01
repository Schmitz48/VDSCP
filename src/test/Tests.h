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


#endif //VDS_PROJECT_TESTS_H

