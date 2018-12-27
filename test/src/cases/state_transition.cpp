/**
 * \file
 * \brief State transition test

 * \author  Nandkishor Biradar
 * \date  15 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

#include "catch.hpp"
#include "hsm.h"
#include "gmock/gmock.h"
#include "gmock-global.h"

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;
namespace state_transition
{

MOCK_GLOBAL_FUNC1(handler1Entry, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler1Exit, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler2Entry, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler2Exit, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler3Entry, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler3Exit, state_machine_result_t(state_machine_t * const));

SCENARIO("Finite state machine transition")
{
  const static state_t testHSM[] =
  {
    {
      NULL,
      handler1Entry,
      handler1Exit,
    },
    {
      NULL,
      handler2Entry,
      handler2Exit
    }
  };

  GIVEN( "A simple finite state machine" )
  {

    state_machine_t machine1;
    machine1.State = testHSM;
    WHEN( "State transition using \"switch_state\"" )
    {
      InSequence s;

      EXPECT_GLOBAL_CALL(handler1Exit, handler1Exit(&machine1));
      EXPECT_GLOBAL_CALL(handler2Entry, handler2Entry(&machine1));

      THEN("It invokes exit handler of source state and entry handler of target state")
      {
        REQUIRE((switch_state(&machine1, &testHSM[1])) == EVENT_HANDLED);
        REQUIRE(machine1.State == &testHSM[1]);
      }
    }
    #if HIERARCHICAL_STATES
    WHEN( "State transitions using \"traverse_state\"" )
    {
      InSequence s;

      EXPECT_GLOBAL_CALL(handler1Exit, handler1Exit(&machine1));
      EXPECT_GLOBAL_CALL(handler2Entry, handler2Entry(&machine1));

      THEN("It invokes exit handler of source state and entry handler of target state")
      {
        REQUIRE((traverse_state(&machine1, &testHSM[1])) == EVENT_HANDLED);
        REQUIRE(machine1.State == &testHSM[1]);
      }
    }
    #endif // HIERARCHICAL_STATES
    WHEN("Entry handler triggers event to self")
    {
      InSequence s;

      EXPECT_GLOBAL_CALL(handler1Exit, handler1Exit(&machine1))
      .WillRepeatedly(Return(TRIGGERED_TO_SELF));
      EXPECT_GLOBAL_CALL(handler2Entry, handler2Entry(&machine1));

      THEN("traverse_state returns TRIGGERED_TO_SELF status")
      {
        REQUIRE((switch_state(&machine1, &testHSM[1])) == TRIGGERED_TO_SELF);
        REQUIRE(machine1.State == &testHSM[1]);
      }
    }
    WHEN("Exit handler triggers event to self")
    {
      InSequence s;

      EXPECT_GLOBAL_CALL(handler1Exit, handler1Exit(&machine1));
      EXPECT_GLOBAL_CALL(handler2Entry, handler2Entry(&machine1))
      .WillRepeatedly(Return(TRIGGERED_TO_SELF));

      THEN("traverse_state returns TRIGGERED_TO_SELF status")
      {
        REQUIRE((switch_state(&machine1, &testHSM[1])) == TRIGGERED_TO_SELF);
        REQUIRE(machine1.State == &testHSM[1]);
      }
    }
    WHEN("Entry handler returns error")
    {
      InSequence s;

      EXPECT_GLOBAL_CALL(handler1Exit, handler1Exit(&machine1))
      .WillRepeatedly(Return(EVENT_UN_HANDLED));

      THEN("traverse_state terminates the state transition")
      {
        REQUIRE((switch_state(&machine1, &testHSM[1])) == EVENT_UN_HANDLED);
      }
    }
    WHEN("Exit handler returns error")
    {
      InSequence s;

      EXPECT_GLOBAL_CALL(handler1Exit, handler1Exit(&machine1));
      EXPECT_GLOBAL_CALL(handler2Entry, handler2Entry(&machine1))
      .WillRepeatedly(Return(EVENT_UN_HANDLED));

      THEN("traverse_state returns TRIGGERED_TO_SELF status")
      {
        REQUIRE((switch_state(&machine1, &testHSM[1])) == EVENT_UN_HANDLED);
      }
    }
  }
}
}

