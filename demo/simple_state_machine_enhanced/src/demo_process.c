/**
 * \file
 * \brief Simple finite state machine example

 * \author  Nandkishor Biradar
 * \date    14 December 2018

 *  Copyright (c) 2018-2019 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

/*
 *  --------------------- INCLUDE FILES ---------------------
 */
#include <stdint.h>
#include <stdio.h>

#include "hsm.h"
#include "demo_process.h"

/*
 *  --------------------- DEFINITION ---------------------
 */

#define ALL_PROCESS_STATE \
 ADD_STATE(IDLE_STATE, idle_handler, idle_entry_handler, idle_exit_handler)  \
 ADD_STATE(ACTIVE_STATE, active_handler, active_entry_handler, active_exit_handler)  \
 ADD_STATE(PAUSE_STATE, paused_handler, paused_entry_handler, paused_exit_handler)

/*
 *  --------------------- ENUMERATION ---------------------
 */

//! List of states in the process state machine
#define ADD_STATE(name, ...)  name,
typedef enum
{
  ALL_PROCESS_STATE
}process_state_t;
#undef ADD_STATE

/*
 *  --------------------- Function prototype ---------------------
 */

static state_machine_result_t idle_handler(state_machine_t* const State);
static state_machine_result_t idle_entry_handler(state_machine_t* const State);
static state_machine_result_t idle_exit_handler(state_machine_t* const State);

static state_machine_result_t active_handler(state_machine_t* const State);
static state_machine_result_t active_entry_handler(state_machine_t* const State);
static state_machine_result_t active_exit_handler(state_machine_t* const State);

static state_machine_result_t paused_handler(state_machine_t* const State);
static state_machine_result_t paused_entry_handler(state_machine_t* const State);
static state_machine_result_t paused_exit_handler(state_machine_t* const State);

/*
 *  --------------------- Global variables ---------------------
 */

#define ADD_STATE(name, state_handler, entry_handler, exit_handler) \
[name] = {                  \
  .Handler = state_handler, \
  .Entry   = entry_handler, \
  .Exit    = exit_handler,  \
  .Id      = name,          \
},

static const state_t Process_States[] =
{
  ALL_PROCESS_STATE
};

#undef ADD_STATE

/*
 *  --------------------- Functions ---------------------
 */

void init_process(process_t* const pProcess, uint32_t processTime)
{
  pProcess->Machine.State = &Process_States[IDLE_STATE];
  pProcess->Machine.Event = 0;
  pProcess->Set_Time = processTime;
  pProcess->Resume_Time = 0;

  idle_entry_handler((state_machine_t *)pProcess);
}

static state_machine_result_t idle_entry_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Timer = 0;  // Stop process timer

  printf("Entering to idle state\n");
  printf("Supported events\n");
  printf("'s' : Start process\n");
  return EVENT_HANDLED;
}

static state_machine_result_t idle_handler(state_machine_t* const pState)
{
  switch(pState->Event)
  {
  case START:
    return switch_state(pState, &Process_States[ACTIVE_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t idle_exit_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Timer = pProcess->Set_Time;
  printf("Exiting from idle state\n");
  return EVENT_HANDLED;
}

static state_machine_result_t active_entry_handler(state_machine_t* const pState)
{
  (void)(pState);
  printf("Entering to active state\n");
  printf("Supported events\n");
  printf("'q' : stop process\n");
  printf("'p' : Pause process\n");
  return EVENT_HANDLED;
}

static state_machine_result_t active_handler(state_machine_t* const pState)
{
  switch(pState->Event)
  {
  case STOP:
    return switch_state(pState, &Process_States[IDLE_STATE]);

  case PAUSE:
    return switch_state(pState, &Process_States[PAUSE_STATE]);

  case TIMEOUT:
    return switch_state(pState, &Process_States[IDLE_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t active_exit_handler(state_machine_t* const pState)
{
  (void)(pState);
  printf("Exiting from Active state\n");
  return EVENT_HANDLED;
}

static state_machine_result_t paused_entry_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Resume_Time = pProcess->Timer;  // Save remaining time
  pProcess->Timer = 0;    // Stop the process timer

  printf("Entering to pause state\n");
  printf("Supported events\n");
  printf("'q' : stop process\n");
  printf("'r' : resume process\n");
  return EVENT_HANDLED;
}

static state_machine_result_t paused_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  switch(pState->Event)
  {
  case STOP:
    return switch_state(pState, &Process_States[IDLE_STATE]);

  case RESUME:
    pProcess->Timer = pProcess->Resume_Time;
    return switch_state(pState, &Process_States[ACTIVE_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t paused_exit_handler(state_machine_t* const pState)
{
  (void)(pState);
  printf("Exiting from paused state\n");
  return EVENT_HANDLED;
}
