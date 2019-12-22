/**
 * \file
 * \brief Hierarchical state traversing test

 * \author  Nandkishor Biradar
 * \date  17 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

#include "catch.hpp"
#define _HIPPOMOCKS__ENABLE_CFUNC_MOCKING_SUPPORT
#include "hippomocks.h"

#include "hsm.h"

namespace hierarchical_state_transition {

state_machine_result_t level1_child1_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level1_child1_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level1_child2_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level1_child2_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level1_child3_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level1_child3_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}

state_machine_result_t level2_child1_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child1_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child2_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child2_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child3_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child3_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child4_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level2_child4_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}

state_machine_result_t level3_child1_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child1_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child2_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child2_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child3_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child3_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child4_entry_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}
state_machine_result_t level3_child4_exit_handler(state_machine_t * const)
{
  return EVENT_HANDLED;
}

extern const state_t Level2_Child1_HSM[];
extern const state_t Level2_Child3_HSM[];
extern const state_t Level3_Child1_HSM[];
extern const state_t Level3_Child3_HSM[];
extern const state_t Level3_Child4_HSM[];

const state_t Level1_HSM[] =
{
  {
    NULL,
    level1_child1_entry_handler,
    level1_child1_exit_handler,
    NULL,
    Level2_Child1_HSM,
    0
  },
  {
    NULL,
    level1_child2_entry_handler,
    level1_child2_exit_handler,
    NULL,
    Level2_Child3_HSM,
    0
  },
  {
    NULL,
    level1_child3_entry_handler,
    level1_child3_exit_handler,
    NULL,
    NULL,
    0
  }
};

const state_t Level2_Child1_HSM[] =
{
  {
    NULL,
    level2_child1_entry_handler,
    level2_child1_exit_handler,
    &Level1_HSM[0],
    NULL,
    1
  },
  {
    NULL,
    level2_child2_entry_handler,
    level2_child2_exit_handler,
    &Level1_HSM[0],
    Level3_Child1_HSM,
    1
  },
};

const state_t Level2_Child3_HSM[] =
{
  {
    NULL,
    level2_child3_entry_handler,
    level2_child3_exit_handler,
    &Level1_HSM[1],
    Level3_Child3_HSM,
    1
  },
  {
    NULL,
    level2_child4_entry_handler,
    level2_child4_exit_handler,
    &Level1_HSM[1],
    Level3_Child4_HSM,
    1
  },
};

const state_t Level3_Child1_HSM[] =
{
  {
    NULL,
    level3_child1_entry_handler,
    level3_child1_exit_handler,
    &Level2_Child1_HSM[1],
    NULL,
    2
  },
  {
    NULL,
    level3_child2_entry_handler,
    level3_child2_exit_handler,
    &Level2_Child1_HSM[1],
    NULL,
    2
  },
};

const state_t Level3_Child3_HSM[] =
{
  {
    NULL,
    level3_child3_entry_handler,
    level3_child3_exit_handler,
    &Level2_Child3_HSM[0],
    NULL,
    2
  }
};

const state_t Level3_Child4_HSM[] =
{
  {
    NULL,
    level3_child4_entry_handler,
    level3_child4_exit_handler,
    &Level2_Child3_HSM[1],
    NULL,
    2
  }
};

SCENARIO("Transition from same level and same parent")
{
  GIVEN( "A composite state machine" )
  {
    state_machine_t machine;
    WHEN("Transition from Level3_Child1 to Level3_Child2")
    {

      machine.State = Level3_Child1_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level3_child1_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level3_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level3_Child1_HSM[1])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level3_Child1_HSM[1]);
      }
    }
  }
}

SCENARIO("Transition from same level and different parent states")
{
  GIVEN( "A composite state machine" )
  {
    state_machine_t machine;
    WHEN("Transition from Level3_Child3 to Level3_Child4")
    {
      AND_WHEN("any of entry handler triggers event to self")
      {
        machine.State = Level3_Child3_HSM;

        MockRepository mocks;
        mocks.ExpectCallFunc(level3_child3_exit_handler).With(&machine).Return(TRIGGERED_TO_SELF);
        mocks.ExpectCallFunc(level2_child3_exit_handler).With(&machine).Return(EVENT_HANDLED);
        mocks.ExpectCallFunc(level2_child4_entry_handler).With(&machine).Return(EVENT_HANDLED);
        mocks.ExpectCallFunc(level3_child4_entry_handler).With(&machine).Return(EVENT_HANDLED);

        THEN("traverse_state invokes respective entry and exit handlers")
        {
          REQUIRE((traverse_state(&machine, Level3_Child4_HSM)) == TRIGGERED_TO_SELF);
          REQUIRE(machine.State == Level3_Child4_HSM);
        }
      }
    }

    WHEN("Transition from Level3_Child4 to Level3_Child2")
    {
      machine.State = Level3_Child4_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level3_child4_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child4_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child2_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child1_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level3_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level3_Child1_HSM[1])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level3_Child1_HSM[1]);
      }
    }
  }
}

SCENARIO("Transition from low level to high level states")
{
  GIVEN( "A composite state machine" )
  {
    state_machine_t machine;
    WHEN("Transition from Level3_Child3 to Level1_Child3")
    {
      machine.State = Level3_Child3_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level3_child3_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child3_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child2_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child3_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level1_HSM[2])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level1_HSM[2]);
      }
    }

    WHEN("Transition from Level3_Child1 to Level2_Child1")
    {
      machine.State = Level3_Child1_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level3_child1_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child2_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child1_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level2_Child1_HSM[0])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level2_Child1_HSM[0]);
      }
    }

    WHEN("Transition from Level3_Child4 to Level2_Child1")
    {
      machine.State = Level3_Child4_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level3_child4_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child4_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child2_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child1_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child1_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level2_Child1_HSM[0])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level2_Child1_HSM[0]);
      }
    }
  }
}

SCENARIO("Transition from high level to low level states")
{
  GIVEN( "A composite state machine" )
  {
    state_machine_t machine;
    WHEN("Transition from Level1_Child3 to Level3_Child2")
    {
      machine.State = &Level1_HSM[2];

      MockRepository mocks;
      mocks.ExpectCallFunc(level1_child3_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child1_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level3_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level3_Child1_HSM[1])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level3_Child1_HSM[1]);
      }
    }

    WHEN("Transition from Level2_Child1 to Level3_Child2")
    {
      machine.State = Level2_Child1_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level2_child1_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level3_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level3_Child1_HSM[1])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level3_Child1_HSM[1]);
      }
    }

    WHEN("Transition from Level2_Child1 to Level3_Child4")
    {
      machine.State = Level2_Child1_HSM;

      MockRepository mocks;
      mocks.ExpectCallFunc(level2_child1_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child1_exit_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level1_child2_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level2_child4_entry_handler).With(&machine).Return(EVENT_HANDLED);
      mocks.ExpectCallFunc(level3_child4_entry_handler).With(&machine).Return(EVENT_HANDLED);

      THEN("traverse_state invokes respective entry and exit handlers")
      {
        REQUIRE((traverse_state(&machine, &Level3_Child4_HSM[0])) == EVENT_HANDLED);
        REQUIRE(machine.State == &Level3_Child4_HSM[0]);
      }
    }
  }
}

}


