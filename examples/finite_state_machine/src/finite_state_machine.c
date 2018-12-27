/**
 * \file
 * \brief Simple finite state machine example

 * \author  Nandkishor Biradar
 * \date    14 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE.txt or copy at https://mit-license.org/)
 */

/*
 *  --------------------- INCLUDE FILES ---------------------
 */
#include <stdint.h>
#include <stdio.h>

#include "hsm.h"
#include "finite_state_machine.h"

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

static const state_t Process_States[] =
{
  [IDLE_STATE].Handler = idle_handler,
  [IDLE_STATE].Entry   = idle_entry_handler,
  [IDLE_STATE].Exit    = idle_exit_handler,
  [IDLE_STATE].Id      = IDLE_STATE,

  [ACTIVE_STATE].Handler = active_handler,
  [ACTIVE_STATE].Entry   = active_entry_handler,
  [ACTIVE_STATE].Exit    = active_exit_handler,
  [ACTIVE_STATE].Id       = ACTIVE_STATE,

  [PAUSE_STATE].Handler = paused_handler,
  [PAUSE_STATE].Entry   = paused_entry_handler,
  [PAUSE_STATE].Exit     = paused_exit_handler,
  [PAUSE_STATE].Id      = PAUSE_STATE,
};

/*
 *  --------------------- Functions ---------------------
 */

void init_process(process_t* const pProcess, uint32_t processTime)
{
  pProcess->State = &Process_States[IDLE_STATE];
  pProcess->Event = 0;
  pProcess->Set_Time = processTime;
  pProcess->Resume_Time = 0;

  idle_entry_handler((state_machine_t *)pProcess);
}

static inline void start_process(process_t* const pProcess)
{
  pProcess->Timer = pProcess->Set_Time * 1000;
}

static inline void resume_process(process_t* const pProcess)
{
  pProcess->Timer = pProcess->Resume_Time;
}

static state_machine_result_t idle_entry_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  pProcess->Timer = 0;  // Stop process timer

  printf("Entering to idle state\n");
  printf("Supported events\n");
  printf("1. 's' : Start process\n");
  return EVENT_HANDLED;
}

static state_machine_result_t idle_handler(state_machine_t* const pState)
{
  switch(pState->Event)
  {
  case START:
  case START_CAPS:
    return switch_finite_state(pState, &Process_States[ACTIVE_STATE]);

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t idle_exit_handler(state_machine_t* const pState)
{
  start_process((process_t*)pState);
  printf("Exiting from idle state\n");
  return EVENT_HANDLED;
}

static state_machine_result_t active_entry_handler(state_machine_t* const pState)
{
  printf("Entering to active state\n");
  printf("Supported events\n");
  printf("1. 'q' : stop process\n");
  printf("2. 'p' : Pause process\n");
  printf("3. 't' : timeout\n" );
  printf("4. 'g' : get time elapsed\n");

  return EVENT_HANDLED;
}

static state_machine_result_t active_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  switch(pState->Event)
  {
  case QUIT:
  case QUIT_CAPS:
    return switch_finite_state(pState, &Process_States[IDLE_STATE]);

  case PAUSE:
  case PAUSE_CAPS:
    return switch_finite_state(pState, &Process_States[PAUSE_STATE]);

  case TIMEOUT:
  case TIMEOUT_CAPS:
    return switch_finite_state(pState, &Process_States[IDLE_STATE]);

  case GET_TIME:
  case GET_TIME_CAPS:
    printf("Remaining time: %d.%d seconds\n", pProcess->Timer / 1000, (pProcess->Timer % 1000) / 100);
    break;

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t active_exit_handler(state_machine_t* const pState)
{
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
  printf("1. 's' : restart process\n");
  printf("2. 'q' : stop process\n");
  printf("3. 'r' : resume process\n");
  printf("4. 'g' : get time elapsed\n");
  return EVENT_HANDLED;
}

static state_machine_result_t paused_handler(state_machine_t* const pState)
{
  process_t* const pProcess = (process_t*)pState;
  switch(pState->Event)
  {
  case START:
  case START_CAPS:
    start_process(pProcess);
    return switch_finite_state(pState, &Process_States[ACTIVE_STATE]);

  case QUIT:
  case QUIT_CAPS:
    return switch_finite_state(pState, &Process_States[IDLE_STATE]);

  case RESUME:
  case RESUME_CAPS:
    resume_process(pProcess);
    return switch_finite_state(pState, &Process_States[ACTIVE_STATE]);

  case GET_TIME:
  case GET_TIME_CAPS:
    printf("Remaining time: %d.%d seconds\n", pProcess->Resume_Time / 1000, (pProcess->Resume_Time % 1000) / 100);
    break;

  default:
    return EVENT_UN_HANDLED;
  }
  return EVENT_HANDLED;
}

static state_machine_result_t paused_exit_handler(state_machine_t* const pState)
{
  printf("Exiting from paused state\n");
  return EVENT_HANDLED;
}
