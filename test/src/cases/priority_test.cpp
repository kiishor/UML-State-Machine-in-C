/**
 * \file
 * \brief Priority test of event dispatcher

 * \author  Nandkishor Biradar
 * \date  05 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE.txt or copy at https://mit-license.org/)
 */

#include "catch.hpp"
#include "hsm.h"
#include "gmock/gmock.h"
#include "gmock-global.h"

namespace priority_test
{

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;

MOCK_GLOBAL_FUNC1(handler1, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler2, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler3, state_machine_result_t(state_machine_t * const));

state_machine_t machine1, machine2, machine3;

state_machine_result_t triggerEvent(state_machine_t * const pstMachine)
{
  machine1.Event = 1;
  machine3.Event = 1;
  return EVENT_HANDLED;
}

state_machine_result_t selfTrigger(state_machine_t * const pstMachine)
{
  machine2.Event = 1;
  return TRIGGERED_TO_SELF;
}

SCENARIO("Priority test ")
{

  const static state_t testHSM[] =
  {
      handler1,
  };

  const static state_t test1HSM[] =
  {
      handler2,
  };

  const static state_t test2HSM[] =
  {
      handler3,
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

      InSequence s;

      EXPECT_GLOBAL_CALL(handler1, handler1(&machine1));
      EXPECT_GLOBAL_CALL(handler2, handler2(&machine2));
      EXPECT_GLOBAL_CALL(handler3, handler3(&machine3));

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

      InSequence s;

      EXPECT_GLOBAL_CALL(handler2, handler2(&machine2))
      .WillOnce(Invoke(&triggerEvent));
      EXPECT_GLOBAL_CALL(handler1, handler1(&machine1));
      EXPECT_GLOBAL_CALL(handler3, handler3(&machine3));

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

      InSequence s;

      EXPECT_GLOBAL_CALL(handler3, handler3(&machine3))
      .WillOnce(Invoke(&selfTrigger));
      EXPECT_GLOBAL_CALL(handler2, handler2(&machine2));
      EXPECT_GLOBAL_CALL(handler3, handler3(&machine3));

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
