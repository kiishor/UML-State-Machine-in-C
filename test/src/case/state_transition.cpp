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
#define _HIPPOMOCKS__ENABLE_CFUNC_MOCKING_SUPPORT
#include "hippomocks.h"

namespace state_transition
{

state_machine_result_t handler1Entry(state_machine_t* const)
{
  return EVENT_HANDLED;
}

state_machine_result_t handler1Exit(state_machine_t* const)
{
  return EVENT_HANDLED;
}

state_machine_result_t handler2Entry(state_machine_t* const)
{
  return EVENT_HANDLED;
}

state_machine_result_t handler2Exit(state_machine_t* const)
{
  return EVENT_HANDLED;
}

SCENARIO("Finite state machine transition")
{
  const static state_t testHSM[] =
  {
    {
      NULL,
      handler1Entry,
      handler1Exit,
      #if HIERARCHICAL_STATES
      NULL,
      NULL,
      0
      #endif
    },
    {
      NULL,
      handler2Entry,
      handler2Exit,
      #if HIERARCHICAL_STATES
      NULL,
      NULL,
      0
      #endif
    }
  };

  GIVEN( "A simple finite state machine" )
  {

    state_machine_t machine;
    machine.State = testHSM;
    WHEN( "State transition using \"switch_state\"" )
    {
      MockRepository mocks;
      mocks.ExpectCallFunc(handler1Exit).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler2Entry).With(&machine).Return(EVENT_HANDLED);

      THEN("It invokes exit handler of source state and entry handler of target state")
      {
        REQUIRE((switch_state(&machine, &testHSM[1])) == EVENT_HANDLED);
        REQUIRE(machine.State == &testHSM[1]);
      }
    }
    #if HIERARCHICAL_STATES
    WHEN( "State transitions using \"traverse_state\"" )
    {
      MockRepository mocks;
      mocks.ExpectCallFunc(handler1Exit).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler2Entry).With(&machine).Return(EVENT_HANDLED);

      THEN("It invokes exit handler of source state and entry handler of target state")
      {
        REQUIRE((traverse_state(&machine, &testHSM[1])) == EVENT_HANDLED);
        REQUIRE(machine.State == &testHSM[1]);
      }
    }
    #endif // HIERARCHICAL_STATES
    WHEN("Entry handler triggers event to self")
    {
      MockRepository mocks;
      mocks.ExpectCallFunc(handler1Exit).With(&machine).Return(TRIGGERED_TO_SELF);
      mocks.ExpectCallFunc(handler2Entry).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state returns TRIGGERED_TO_SELF status")
      {
        REQUIRE((switch_state(&machine, &testHSM[1])) == TRIGGERED_TO_SELF);
        REQUIRE(machine.State == &testHSM[1]);
      }
    }
    WHEN("Exit handler triggers event to self")
    {
      MockRepository mocks;
      mocks.ExpectCallFunc(handler1Exit).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler2Entry).With(&machine).Return(TRIGGERED_TO_SELF);

      THEN("traverse_state returns TRIGGERED_TO_SELF status")
      {
        REQUIRE((switch_state(&machine, &testHSM[1])) == TRIGGERED_TO_SELF);
        REQUIRE(machine.State == &testHSM[1]);
      }
    }
    WHEN("Entry handler returns error")
    {
      MockRepository mocks;
      mocks.ExpectCallFunc(handler1Exit).With(&machine).Return(EVENT_UN_HANDLED);

      THEN("traverse_state terminates the state transition")
      {
        REQUIRE((switch_state(&machine, &testHSM[1])) == EVENT_UN_HANDLED);
      }
    }
    WHEN("Exit handler returns error")
    {
      MockRepository mocks;
      mocks.ExpectCallFunc(handler1Exit).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler2Entry).With(&machine).Return(EVENT_UN_HANDLED);

      THEN("traverse_state returns TRIGGERED_TO_SELF status")
      {
        REQUIRE((switch_state(&machine, &testHSM[1])) == EVENT_UN_HANDLED);
      }
    }
  }
}
}

