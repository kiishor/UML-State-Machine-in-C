/**
 * \file
 * \brief Oven hierarchical state machine example

 * \author  Nandkishor Biradar
 * \date    03 February 2020

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
#include "toaster_oven.h"

/*
 *  --------------------- DEFINITION ---------------------
 */

#define ALL_OVEN_STATES	\
	ADD_ROOT_LEAF(DOOR_OPEN_STATE, door_open_handler, door_open_entry_handler, NULL)	\
	ADD_ROOT(DOOR_CLOSE_STATE, NULL, door_close_entry_handler, NULL, Door_Close_State) \

#define ALL_DOOR_CLOSED_STATES(PARENT, LEVEL)	\
	ADD_LEAF(OFF_STATE, off_handler, off_entry_handler, NULL, PARENT, LEVEL)	\
	ADD_LEAF(ON_STATE, on_handler, on_entry_handler, on_exit_handler, PARENT, LEVEL)

/*
 *  --------------------- ENUMERATION ---------------------
 */

#define ADD_NODE(NAME, ...)		NAME,
#define ADD_ROOT(NAME, ...)	NAME,
#define ADD_ROOT_LEAF(NAME, ...)	NAME,
#define ADD_LEAF(NAME, ...)	NAME,

//! List of states in oven state machine
typedef enum
{
  ALL_OVEN_STATES
}oven_state_t;

//! List of states in door closed states machine
typedef enum
{
  ALL_DOOR_CLOSED_STATES(,)
  TOTAL_DOOR_CLOSED_STATES
}door_close_state_t;

#undef ADD_NODE
#undef ADD_ROOT
#undef ADD_ROOT_LEAF
#undef ADD_LEAF

/*
 *  --------------------- Function prototype ---------------------
 */

static state_machine_result_t door_open_handler(state_machine_t* const pState);
static state_machine_result_t door_open_entry_handler(state_machine_t* const pState);

static state_machine_result_t door_close_entry_handler(state_machine_t* const pState);

static state_machine_result_t off_handler(state_machine_t* const pState);
static state_machine_result_t off_entry_handler(state_machine_t* const pState);
static state_machine_result_t on_handler(state_machine_t* const pState);
static state_machine_result_t on_entry_handler(state_machine_t* const pState);
static state_machine_result_t on_exit_handler(state_machine_t* const pState);

/*
 *  --------------------- Global variables ---------------------
 */

 #define ADD_ROOT(NAME, HANDLER, ENTRY, EXIT, CHILD)	\
	[NAME] = {                          \
	  .Handler 	= HANDLER,							\
    .Entry	 	= ENTRY,								\
    .Exit 		= EXIT,									\
    .Level		= 0,										\
    .Node			= CHILD,								\
	},

#define ADD_ROOT_LEAF(NAME, HANDLER, ENTRY, EXIT)		\
	[NAME] = {                              \
	  .Handler 	= HANDLER,									\
    .Entry	 	= ENTRY,										\
    .Exit 		= EXIT,											\
	},

#define ADD_LEAF(NAME, HANDLER, ENTRY, EXIT, PARENT, LEVEL) \
	[NAME] = {                              \
	  .Handler 	= HANDLER,									\
    .Entry	 	= ENTRY,										\
    .Exit 		= EXIT,											\
    .Level		= 1,                        \
    .Parent   = PARENT,                   \
	},

static const state_t Door_Close_State[TOTAL_DOOR_CLOSED_STATES];

//! List of all the states
static const state_t Oven_State[] =
{
  ALL_OVEN_STATES
};

static const state_t Door_Close_State[TOTAL_DOOR_CLOSED_STATES] =
{
	ALL_DOOR_CLOSED_STATES(&Oven_State[DOOR_CLOSE_STATE], 1)
};

#undef ADD_ROOT
#undef ADD_ROOT_LEAF
#undef ADD_LEAF

/*
 *  --------------------- Functions ---------------------
 */

void init_oven(oven_t* const pOven, uint32_t toastTime, door_status_t status)
{
  if(status == DOOR_CLOSED)
  {
    pOven->Machine.State = &Door_Close_State[OFF_STATE];
  }
  else
  {
    pOven->Machine.State = &Oven_State[DOOR_OPEN_STATE];
  }
  pOven->Machine.Event = 0;
  pOven->Set_Time = toastTime;
  pOven->Resume_Time = 0;
  pOven->Timer = 0;
  pOven->Lamp = false;
  pOven->Heater = false;

  printf("Oven is initialized\n");
  printf("Door is closed and oven is off\n");
  printf("Press 's': to turn on Oven\n");
  printf("Press 'o': to open the door\n");
}

static state_machine_result_t door_open_handler(state_machine_t* const pState)
{
  oven_t* const pOven = (oven_t*)pState;
  switch(pState->Event)
  {
  case EN_DOOR_CLOSE:
    if(pOven->Resume_Time)
    {
      return traverse_state(pState, &Door_Close_State[ON_STATE]);
    }
    else
    {
      return traverse_state(pState, &Door_Close_State[OFF_STATE]);
    }

  default:
    return EVENT_UN_HANDLED;
  }
}

static state_machine_result_t door_open_entry_handler(state_machine_t* const pState)
{
  printf("Turn on Oven lamp\n");
  ((oven_t*)pState)->Lamp = true;

  printf("Press 'c': to close the door\n");
  return EVENT_HANDLED;
}

static state_machine_result_t door_close_entry_handler(state_machine_t* const pState)
{
  printf("Turn off Oven lamp\n");
  ((oven_t*)pState)->Lamp = false;
  return EVENT_HANDLED;
}

static state_machine_result_t off_handler(state_machine_t* const pState)
{
  oven_t* const pOven = (oven_t*)pState;
  switch(pState->Event)
  {
  case EN_START:
    pOven->Timer = pOven->Set_Time;
    pOven->Resume_Time = 0;
    // Use switch_state when target state is within the same state machine.
    // i.e. target and source states have same parent state.
    return switch_state(pState, &Door_Close_State[ON_STATE]);

  case EN_DOOR_OPEN:
    // Use traverse_state when target and source states doesn't have same parent state.
      return traverse_state(pState, &Oven_State[DOOR_OPEN_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
}

static state_machine_result_t off_entry_handler(state_machine_t* const pState)
{
  (void)(pState);
  printf("Press 's': to turn on Oven\n");
  printf("Press 'o': to open the door\n");
  return EVENT_HANDLED;
}

static state_machine_result_t on_handler(state_machine_t* const pState)
{
  oven_t* const pOven = (oven_t*)pState;
  switch(pState->Event)
  {
  case EN_STOP:
    pOven->Timer = 0;
    pOven->Resume_Time = 0;
    return switch_state(pState, &Door_Close_State[OFF_STATE]);

  case EN_TIMEOUT:
    return switch_state(pState, &Door_Close_State[OFF_STATE]);

  case EN_DOOR_OPEN:
    pOven->Resume_Time = pOven->Timer;
    pOven->Timer = 0;
    return traverse_state(pState, &Oven_State[DOOR_OPENED]);

  default:
    return EVENT_UN_HANDLED;
  }
}

static state_machine_result_t on_entry_handler(state_machine_t* const pState)
{
  oven_t* const pOven = (oven_t*)pState;
  printf("Turn on heater\n");
  pOven->Heater = true;

  if(pOven->Resume_Time)
  {
    pOven->Timer = pOven->Resume_Time;
    pOven->Resume_Time = 0;
  }
  printf("Press 'q': to turn off Oven\n");
  printf("Press 'o': to open the door\n");

  return EVENT_HANDLED;
}

static state_machine_result_t on_exit_handler(state_machine_t* const pState)
{
  oven_t* const pOven = (oven_t*)pState;
  printf("Turn Off heater\n");
  pOven->Heater = false;
  return EVENT_HANDLED;
}
