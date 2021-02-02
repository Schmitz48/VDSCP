//
// Created by tobias on 29.11.18.
//

#ifndef VDSPROJECT_TESTS_H
#define VDSPROJECT_TESTS_H

#include "Reachable.h"
#include "gtest/gtest.h"
using namespace ClassProject;

TEST(managerTest, HowTo_Example) {

    ClassProject::Reachable comp(2);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;

    auto s0 = states.at(0);
    auto s1 = states.at(1);
    //s0' = not(s0)
    functions.push_back(comp.getManager()->neg(s0));
    //s1' = not(s1)
    functions.push_back(comp.getManager()->neg(s1));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false});

    comp.compute_reachable_states();

    ASSERT_TRUE(comp.is_reachable({true,true}));
    ASSERT_TRUE(comp.is_reachable({false,false}));
    ASSERT_FALSE(comp.is_reachable({true,false}));
    ASSERT_FALSE(comp.is_reachable({false,true}));

}

TEST(managerTest, StateCreation) {

    int numberOfStates = 3;
    ClassProject::Reachable comp(numberOfStates);

    auto states = comp.getStates();
    //The state vector contains 2*numberOfStates state bits
    //One for si and one for si'
    EXPECT_EQ(states.size() ,numberOfStates*2);


}

TEST(managerTest, XNOR) {

    int numberOfStates = 3;
    ClassProject::Reachable comp(numberOfStates);

    //Test trivial cases for xnor
    EXPECT_EQ(comp.xnor2(1,1) ,1);
    EXPECT_EQ(comp.xnor2(0,0) ,1);
    EXPECT_EQ(comp.xnor2(0,1) ,0);
    EXPECT_EQ(comp.xnor2(1,0) ,0);

    auto states = comp.getStates();

    BDD_ID s0 = states.at(0);
    BDD_ID s1 = states.at(1);
    auto id = comp.xnor2(s0,s1);
    auto trues0 = comp.getManager()->coFactorTrue(id,s0);
    auto trues1 = comp.getManager()->coFactorTrue(trues0,s1);
    auto falses1 = comp.getManager()->coFactorFalse(trues0,s1);
    EXPECT_EQ(falses1,0);
    EXPECT_EQ(trues1,1);

}

#endif //VDSPROJECT_TESTS_H
