/**
 * \file
 * \brief Priority test of event dispatcher

 * \author  Nandkishor Biradar
 * \date  05 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

#include "catch.hpp"
#define _HIPPOMOCKS__ENABLE_CFUNC_MOCKING_SUPPORT
#include "hippomocks.h"

#include "hsm.h"

namespace priority_test
{

state_machine_result_t handler1(state_machine_t * const)
{
  return EVENT_HANDLED;
}

state_machine_result_t handler2(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t handler3(state_machine_t * const)
{
  return EVENT_HANDLED;
}

state_machine_t machine1, machine2, machine3;

state_machine_result_t triggerEvent(state_machine_t * const)
{
   machine1.Event = 1;
  machine3.Event = 1;
  return EVENT_HANDLED;
}

state_machine_result_t selfTrigger(state_machine_t * const)
{
  machine2.Event = 1;
  return TRIGGERED_TO_SELF;
}

SCENARIO("Priority test ")
{

  const static state_t testHSM[] =
  {
    handler1,
    NULL,
    NULL,
    NULL,
    NULL,
    0
  };

  const static state_t test1HSM[] =
  {
      handler2,
      NULL,
      NULL,
      NULL,
      NULL,
      0
  };

  const static state_t test2HSM[] =
  {
    handler3,
    NULL,
    NULL,
    NULL,
    NULL,
    0
  };

  typedef enum
  {
    EN_EVENT1 = 1,
    EN_EVENT2,
    EN_EVENT3,
  }en_event;

  GIVEN( "Three state machines " )
  {
    machine1.State = &testHSM[0];
    machine2.State = &test1HSM[0];
    machine3.State = &test2HSM[0];

    // Index of state machine in the array decides the priority.
    // Lower the index higher the priority of state machine.
    state_machine_t * const machineList[] = {&machine1, &machine2, &machine3};

    WHEN( "All state machines are triggered" )
    {
      machine1.Event = EN_EVENT1;
      machine2.Event = EN_EVENT2;
      machine3.Event = EN_EVENT3;

      MockRepository mocks;
      mocks.ExpectCallFunc(handler1).With(&machine1).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler2).With(&machine2).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler3).With(&machine3).Return(EVENT_HANDLED);

      THEN( "It invokes handler as per priority of state machines" )
      {
        REQUIRE(dispatch_event(machineList, sizeof(machineList)/sizeof(machineList[0])) == EVENT_HANDLED);
        REQUIRE(machine1.Event == 0);
        REQUIRE(machine2.Event == 0);
        REQUIRE(machine3.Event == 0);
      }
    }

    WHEN("Medium priority machine triggers events of higher and lower priority machines")
    {
      machine2.Event = EN_EVENT1;

      MockRepository mocks;
      mocks.ExpectCallFunc(handler2).With(&machine2).Do(triggerEvent);
      mocks.ExpectCallFunc(handler1).With(&machine1).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler3).With(&machine3).Return(EVENT_HANDLED);

      THEN("event handler calls higher priority handler before lower priority handler")
      {
        REQUIRE(dispatch_event(machineList, sizeof(machineList)/sizeof(machineList[0])) == EVENT_HANDLED);
        REQUIRE(machine1.Event == 0);
        REQUIRE(machine2.Event == 0);
        REQUIRE(machine3.Event == 0);
      }
    }

    WHEN("a lower priority machine triggers event to self and to a higher priority machine")
    {
      machine3.Event = EN_EVENT1;

      MockRepository mocks;
      mocks.ExpectCallFunc(handler3).With(&machine3).Do(selfTrigger);
      mocks.ExpectCallFunc(handler2).With(&machine2).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(handler3).With(&machine3).Return(EVENT_HANDLED);

      THEN("event handler calls higher priority handler before lower priority handler")
      {
        REQUIRE(dispatch_event(machineList, sizeof(machineList)/sizeof(machineList[0])) == EVENT_HANDLED);
        REQUIRE(machine1.Event == 0);
        REQUIRE(machine2.Event == 0);
        REQUIRE(machine3.Event == 0);
      }
    }
  }
}

}
