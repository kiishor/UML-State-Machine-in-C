/**
 * \file
 * \brief hierarchical state machine demo

 * \author  Nandkishor Biradar
 * \date    12 February 2020

 *  Copyright (c) 2018-2020 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

/*
 *  --------------------- INCLUDE FILES ---------------------
 */
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

#include "hsm.h"
#include "demo.h"

/*
 *  --------------------- Function prototype ---------------------
 */

static state_machine_result_t level1_state1_handler(state_machine_t * const);
static state_machine_result_t level1_state1_entry_handler(state_machine_t * const);
static state_machine_result_t level1_state1_exit_handler(state_machine_t * const);

static state_machine_result_t level1_state2_handler(state_machine_t * const);
static state_machine_result_t level1_state2_entry_handler(state_machine_t * const);
static state_machine_result_t level1_state2_exit_handler(state_machine_t * const);

static state_machine_result_t level2_state1_handler(state_machine_t * const);
static state_machine_result_t level2_state1_entry_handler(state_machine_t * const);
static state_machine_result_t level2_state1_exit_handler(state_machine_t * const);

static state_machine_result_t level3_state1_handler(state_machine_t * const);
static state_machine_result_t level3_state1_entry_handler(state_machine_t * const);
static state_machine_result_t level3_state1_exit_handler(state_machine_t * const);

/*
 *  --------------------- Global variables ---------------------
 */

static const state_t Level2_HSM[1];
static const state_t Level3_HSM[1];

// This is an array of root (top most) states .
static const state_t Level1_HSM[] =
{
  {
    level1_state1_handler,          // state handler
    level1_state1_entry_handler,    // Entry action handler
    level1_state1_exit_handler,     // Exit action handler
    NULL,                           // Parent state
    NULL,                           // Child states
    0                               // Hierarchical state level
  },
  {
    level1_state2_handler,          // state handler
    level1_state2_entry_handler,    // Entry action handler
    level1_state2_exit_handler,     // Exit action handler
    NULL,                           // Parent state
    Level2_HSM,                     // Child state
    0                               // Hierarchical state level
  }
};

// This is an array of substates of Level1_HSM[1] composite state.
static const state_t Level2_HSM[] =
{
  {
    level2_state1_handler,          // state handler
    level2_state1_entry_handler,    // Entry action handler
    level2_state1_exit_handler,     // Exit action handler
    &Level1_HSM[1],                 // Parent state
    &Level3_HSM[0],                 // Child state
    1                               // Hierarchical state level
  }
};

// This is an array of substates of Level2_HSM[0] composite state.
static const state_t Level3_HSM[] =
{
  {
    level3_state1_handler,          // state handler
    level3_state1_entry_handler,    // Entry action handler
    level3_state1_exit_handler,     // Exit action handler
    &Level2_HSM[0],                 // Parent state
    NULL,                           // Child state
    2                               // Hierarchical state level
  }
};

/*
 *  --------------------- Functions ---------------------
 */

void init_demo(demo_state_machine_t* const pDemo)
{
  pDemo->Machine.State = &Level1_HSM[0];
  pDemo->Machine.Event = 0;
  printf("State machine is in L1_S1 state\n");
  printf("Supported Events are\n");
  printf("EVENT1: press 1 to trigger\n");
}

static state_machine_result_t level1_state1_handler(state_machine_t * const pState)
{
  switch(pState->Event)
  {
  case EVENT1:
    printf("L1_S1: event-1 is processed successfully\n");
    return traverse_state(pState, &Level3_HSM[0]);

  default:
    printf("L1_S1: handler does not handle this event\n");
    return EVENT_UN_HANDLED;
  }
}

static state_machine_result_t level1_state1_entry_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L1_S1: entry handler called\n");
  printf("\nState machine is in L1_S1 state\n");
  printf("Supported Events are\n");
  printf("EVENT1: press 1 to trigger\n");
  return EVENT_HANDLED;
}

static state_machine_result_t level1_state1_exit_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L1_S1: exit handler called\n");
  return EVENT_HANDLED;
}

static state_machine_result_t level1_state2_handler(state_machine_t * const pState)
{
  switch(pState->Event)
  {
  case EVENT3:
    printf("L1_S2: event-3 is processed successfully\n");
    return traverse_state(pState, &Level1_HSM[0]);

  default:
    printf("L1_S2: handler does not handle this event\n");
    return EVENT_UN_HANDLED;
  }
}

static state_machine_result_t level1_state2_entry_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L1_S2: entry handler called\n");
  return EVENT_HANDLED;
}

static state_machine_result_t level1_state2_exit_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L1_S2: exit handler called\n");
  return EVENT_HANDLED;
}

static state_machine_result_t level2_state1_handler(state_machine_t * const pState)
{
  switch(pState->Event)
  {
  case EVENT2:
    printf("L2_S1: event-2 is processed successfully\n");
    return traverse_state(pState, &Level1_HSM[0]);

  default:
    printf("L2_S1: handler does not handle this event\n");
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t level2_state1_entry_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L2_S1: entry handler called\n");
  return EVENT_HANDLED;
}

static state_machine_result_t level2_state1_exit_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L2_S1: exit handler called\n");
  return EVENT_HANDLED;
}

static state_machine_result_t level3_state1_handler(state_machine_t * const pState)
{
  switch(pState->Event)
  {
  case EVENT1:
    printf("L3_S1: event-1 is processed successfully\n");
    return traverse_state(pState, &Level1_HSM[0]);

  default:
    printf("L3_S1: handler does not handle this event\n");
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t level3_state1_entry_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L3_S1: entry handler called\n");

  printf("\nState machine is in L3_S1 state\n");
  printf("Supported Events are\n");
  printf("EVENT1: press 1 to trigger\n");
  printf("EVENT2: press 2 to trigger\n");
  printf("EVENT3: press 3 to trigger\n");

  return EVENT_HANDLED;
}

static state_machine_result_t level3_state1_exit_handler(state_machine_t * const pState)
{
  (void)(pState);
  printf("L3_S1: exit handler called\n");
  return EVENT_HANDLED;
}
