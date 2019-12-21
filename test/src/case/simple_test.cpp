/**
 * \file
 * \brief Event dispatch test

 * \author  Nandkishor Biradar
 * \date  09 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */
#include "catch.hpp"
#define _HIPPOMOCKS__ENABLE_CFUNC_MOCKING_SUPPORT
#include "hippomocks.h"

#include "hsm.h"

namespace simple_test
{

state_machine_result_t handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}

SCENARIO("Simple State machine handling event")
{
  const state_t testHSM[1] =
  {
    handler,
    NULL,
    NULL,
    #if HIERARCHICAL_STATES
    NULL,
    NULL,
    0
    #endif
  };

  GIVEN( "A simple state machine" )
  {
    state_machine_t machine;
    state_machine_t * const machineList[] = {&machine};

    WHEN( "event is triggered" )
    {
      machine.State = testHSM;    // initial state
      machine.Event = 1;          // trigger event to state machine

      MockRepository mocks;
//      // Verify that state machines calls the handler for the event.
      mocks.ExpectCallFunc(handler).With(&machine).Do(
        [&testHSM](state_machine_t * const pMachine)
        {
          REQUIRE(pMachine->Event == 1);
          REQUIRE(pMachine->State == testHSM);
          return EVENT_HANDLED;
        });

      THEN( "It invokes the state handler " )
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_HANDLED);
        REQUIRE(machine.Event == 0);
      }
    }

    WHEN("Handler couldn't handle the event")
    {
      machine.State = &testHSM[0];
      machine.Event = 1;

      MockRepository mocks;

      // handler returns  error
      mocks.ExpectCallFunc(handler).With(&machine).Do(
        [&testHSM](state_machine_t * const pMachine)
        {
          REQUIRE(pMachine->Event == 1);
          REQUIRE(pMachine->State == testHSM);
          return EVENT_UN_HANDLED;
        });

      THEN( " event handler returns error" )
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_UN_HANDLED);
      }
    }

    WHEN("Handler triggers the event to self")
    {
      machine.State = &testHSM[0];
      machine.Event = 1;

      MockRepository mocks;
      // Verify that state machines calls the handler for the event.
      mocks.ExpectCallFunc(handler).With(&machine).Do(
        [&testHSM](state_machine_t * const pMachine)
        {
          REQUIRE(pMachine->Event == 1);
          REQUIRE(pMachine->State == testHSM);
          pMachine->Event = 2;
          return TRIGGERED_TO_SELF;
        });

      mocks.ExpectCallFunc(handler).With(&machine).Do(
        [&testHSM](state_machine_t * const pMachine)
        {
          REQUIRE(pMachine->Event == 2);
          REQUIRE(pMachine->State == testHSM);
          return EVENT_HANDLED;
        });

      THEN( "Handler is again invoked with newly triggered event" )
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_HANDLED);
        REQUIRE(machine.Event == 0);
      }
    }
  }
}

}
