/**
 * \file
 * \brief Event handling of hierarchical state machine

 * \author  Nandkishor Biradar
 * \date  20 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

#include "catch.hpp"
#define _HIPPOMOCKS__ENABLE_CFUNC_MOCKING_SUPPORT
#include "hippomocks.h"

#include "hsm.h"

namespace hierarchical_test
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

extern const state_t childHSM[1];
const state_t rootHSM =
{
  handler1,
  NULL,
  NULL,
  NULL,
  childHSM,
  0
};

const state_t childHSM[1] =
{
  {
    handler2,
    NULL,
    NULL,
    &rootHSM,
    NULL,
    1
  }
};

const state_t child1HSM[1] =
{
  {
    handler3,
    NULL,
    NULL,
    childHSM,
    NULL,
    1
  }
};

SCENARIO("Composite state machine test")
{

typedef enum
{
  EN_EVENT1 = 1,
  EN_EVENT2,
  EN_EVENT3,
  EN_EVENT4
}en_event;

  GIVEN( "A composite state machine" )
  {
    state_machine_t machine;
    state_machine_t * const machineList[] = {&machine};
    machine.State = child1HSM;

    WHEN("handler couldn't handle the event")
    {
      machine.Event = EN_EVENT1;

      MockRepository mocks;
      mocks.ExpectCallFunc(handler3).With(&machine).Return(EVENT_UN_HANDLED);
      mocks.ExpectCallFunc(handler2).With(&machine).Return(EVENT_UN_HANDLED);
      mocks.ExpectCallFunc(handler1).With(&machine).Return(EVENT_HANDLED);

      THEN( "Machine invokes the parent handler")
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_HANDLED);
        REQUIRE(machine.Event == 0);
      }
    }
    WHEN("All the handler couldn't handle the event")
    {
      machine.Event = EN_EVENT1;
      MockRepository mocks;
      mocks.ExpectCallFunc(handler3).With(&machine).Return(EVENT_UN_HANDLED);
      mocks.ExpectCallFunc(handler2).With(&machine).Return(EVENT_UN_HANDLED);
      mocks.ExpectCallFunc(handler1).With(&machine).Return(EVENT_UN_HANDLED);

      THEN( "Machine invokes the parent handler")
      {
        REQUIRE(dispatch_event(machineList, 1) == EVENT_UN_HANDLED);
        REQUIRE(machine.Event == EN_EVENT1);
      }
    }
  }
}

}


