/**
 * \file
 * \brief Event dispatch test

 * \author  Nandkishor Biradar
 * \date  09 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE.txt or copy at https://mit-license.org/)
 */
 #include "catch.hpp"
#include "hsm.h"
#include "gmock/gmock.h"
#include "gmock-global.h"

using ::testing::Return;
using ::testing::Field;
using ::testing::AllOf;
using ::testing::Invoke;
using ::testing::InSequence;

namespace simple_test
{

MOCK_GLOBAL_FUNC1(handler, state_machine_result_t(state_machine_t * const));

state_machine_result_t selfTrigger(state_machine_t * const pMachine)
{
  REQUIRE(pMachine->Event == 1);
  pMachine->Event = 2;
  return TRIGGERED_TO_SELF;
}

SCENARIO("Simple State machine handling event")
{
  const state_t testHSM[1] =
  {
    handler
  };

  GIVEN( "A simple state machine" )
  {
    state_machine_t machine;
    state_machine_t * const machineList[] = {&machine};

    WHEN( "event is triggered" )
    {
      machine.State = testHSM;    // initial state
      machine.Event = 1;          // trigger event to state machine

      // Verify that state machines calls the handler for the event.
      EXPECT_GLOBAL_CALL(handler,
                         handler(AllOf((&machine), Field(&state_machine_t::State, testHSM), Field(&state_machine_t::Event, 1))));

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

      // handler returns  error
      EXPECT_GLOBAL_CALL(handler, handler(&machine))
      .WillOnce(Return(EVENT_UN_HANDLED));

      THEN( " event handler returns error" )
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_UN_HANDLED);
      }
    }

    WHEN("Handler triggers the event to self")
    {
      machine.State = &testHSM[0];
      machine.Event = 1;
      InSequence s;

      EXPECT_GLOBAL_CALL(handler, handler(AllOf((&machine), Field(&state_machine_t::Event, 1))))
      .WillOnce(Invoke(&selfTrigger));
      EXPECT_GLOBAL_CALL(handler, handler(AllOf((&machine), Field(&state_machine_t::Event, 2))));

      THEN( "Handler is again invoked with newly triggered event" )
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_HANDLED);
        REQUIRE(machine.Event == 0);
      }
    }
  }
}

}
