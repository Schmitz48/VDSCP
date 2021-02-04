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

/**
 * Truth Table
 * More extensive Test for the Reachability algorithm
 * |    s3   |   s2  |   s1  |   s0  |   s3' |   s2' |   s1' |   s0' |
 *
 * |    0    |   0   |   0   |   0   |   0   |   1   |   1   |   0   |
 * |    0    |   0   |   0   |   1   |   0   |   1   |   1   |   0   |
 * |    0    |   0   |   1   |   0   |   0   |   1   |   1   |   0   |
 * |    0    |   0   |   1   |   1   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   0   |   0   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   0   |   1   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   1   |   0   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   1   |   1   |   0   |   1   |   1   |   0   |
 * |    1    |   0   |   0   |   0   |   0   |   1   |   0   |   0   |
 * |    1    |   0   |   0   |   1   |   0   |   1   |   0   |   0   |
 * |    1    |   0   |   1   |   0   |   0   |   1   |   0   |   0   |
 * |    1    |   0   |   1   |   1   |   0   |   0   |   0   |   1   |
 * |    1    |   1   |   0   |   0   |   0   |   1   |   0   |   1   |
 * |    1    |   1   |   0   |   1   |   0   |   1   |   0   |   1   |
 * |    1    |   1   |   1   |   0   |   0   |   1   |   0   |   1   |
 * |    1    |   1   |   1   |   1   |   0   |   0   |   0   |  0    |
 */
TEST(managerTest, FourState_Example) {

    ClassProject::Reachable comp(4);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;
    auto m = comp.getManager();
    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    auto s3 = states.at(3);
    //s0' = s2 xor s0s1
    functions.push_back(m->xor2(s2, m->and2(s0,s1)));
    //s1' = (!(s0s1s3)+s3)*!s3
    functions.push_back(m->and2(m->neg(s3), m->or2(s3, m->nand2(s3, m->and2(s0,s1)))));
    //s2' = (!(s0s1s3)
    functions.push_back(m->nand2(s3, m->and2(s0,s1)));
    //s3' = (!(s0s1s3)+s3)*!s3 * (s2 xor s0s1)
    functions.push_back(m->and2(m->xor2(s2, comp.getManager()->and2(s0,s1)), m->and2(m->neg(s3), m->or2(s3, m->nand2(s3, m->and2(s0,s1))))));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false, false, false});

    comp.compute_reachable_states();
    //Reachable states have been calculated by hand
    ASSERT_TRUE(comp.is_reachable({false,false, false, false}));
    ASSERT_TRUE(comp.is_reachable({false,true, true, false}));
    ASSERT_TRUE(comp.is_reachable({true,true, true, true}));
    ASSERT_FALSE(comp.is_reachable({false,false, false, true}));
    ASSERT_FALSE(comp.is_reachable({false,false, true, false}));
    ASSERT_FALSE(comp.is_reachable({false,false, true, true}));
    ASSERT_FALSE(comp.is_reachable({false,true, false, false}));
    ASSERT_FALSE(comp.is_reachable({false,true, false, true}));
    ASSERT_FALSE(comp.is_reachable({false,true, true, true}));
    ASSERT_FALSE(comp.is_reachable({true,false, false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, false, true}));
    ASSERT_FALSE(comp.is_reachable({true,false, true, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, true, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, false, false}));
    ASSERT_FALSE(comp.is_reachable({true,true, false, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, true, false}));



}

/**
 * Truth Table
 * More extensive Test for the Reachability algorithm
 * |    s3   |   s2  |   s1  |   s0  |   s3' |   s2' |   s1' |   s0' |
 *
 * |    0    |   0   |   0   |   0   |   0   |   1   |   1   |   0   |
 * |    0    |   0   |   0   |   1   |   0   |   1   |   1   |   0   |
 * |    0    |   0   |   1   |   0   |   0   |   1   |   1   |   0   |
 * |    0    |   0   |   1   |   1   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   0   |   0   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   0   |   1   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   1   |   0   |   1   |   1   |   1   |   1   |
 * |    0    |   1   |   1   |   1   |   0   |   1   |   1   |   0   |
 * |    1    |   0   |   0   |   0   |   0   |   1   |   0   |   0   |
 * |    1    |   0   |   0   |   1   |   0   |   1   |   0   |   0   |
 * |    1    |   0   |   1   |   0   |   0   |   1   |   0   |   0   |
 * |    1    |   0   |   1   |   1   |   0   |   0   |   0   |   1   |
 * |    1    |   1   |   0   |   0   |   0   |   1   |   0   |   1   |
 * |    1    |   1   |   0   |   1   |   0   |   1   |   0   |   1   |
 * |    1    |   1   |   1   |   0   |   0   |   1   |   0   |   1   |
 * |    1    |   1   |   1   |   1   |   0   |   0   |   0   |  0    |
 */
TEST(managerTest, FourState_Example_OtherInitial) {

    ClassProject::Reachable comp(4);

    auto states = comp.getStates();
    std::vector<BDD_ID> functions;
    auto m = comp.getManager();
    auto s0 = states.at(0);
    auto s1 = states.at(1);
    auto s2 = states.at(2);
    auto s3 = states.at(3);
    //s0' = s2 xor s0s1
    functions.push_back(m->xor2(s2, m->and2(s0,s1)));
    //s1' = (!(s0s1s3)+s3)*!s3
    functions.push_back(m->and2(m->neg(s3), m->or2(s3, m->nand2(s3, m->and2(s0,s1)))));
    //s2' = (!(s0s1s3)
    functions.push_back(m->nand2(s3, m->and2(s0,s1)));
    //s3' = (!(s0s1s3)+s3)*!s3 * (s2 xor s0s1)
    functions.push_back(m->and2(m->xor2(s2, comp.getManager()->and2(s0,s1)), m->and2(m->neg(s3), m->or2(s3, m->nand2(s3, m->and2(s0,s1))))));
    //Add transition functions
    comp.setDelta(functions);
    //Add init state
    comp.setInitState({false,false, false, true});

    comp.compute_reachable_states();
    //Reachable states have been calculated by hand
    ASSERT_TRUE(comp.is_reachable({false,false, false, false}));
    ASSERT_TRUE(comp.is_reachable({false,false, false, true}));
    ASSERT_TRUE(comp.is_reachable({false,false, true, false}));
    ASSERT_FALSE(comp.is_reachable({false,false, true, true}));
    ASSERT_FALSE(comp.is_reachable({false,true, false, false}));
    ASSERT_FALSE(comp.is_reachable({false,true, false, true}));
    ASSERT_TRUE(comp.is_reachable({false,true, true, false}));
    ASSERT_FALSE(comp.is_reachable({false,true, true, true}));
    ASSERT_FALSE(comp.is_reachable({true,false, false, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, false, true}));
    ASSERT_FALSE(comp.is_reachable({true,false, true, false}));
    ASSERT_FALSE(comp.is_reachable({true,false, true, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, false, false}));
    ASSERT_FALSE(comp.is_reachable({true,true, false, true}));
    ASSERT_FALSE(comp.is_reachable({true,true, true, false}));
    ASSERT_TRUE(comp.is_reachable({true,true, true, true}));



}

#endif //VDSPROJECT_TESTS_H
