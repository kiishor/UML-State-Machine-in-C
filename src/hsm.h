/**
 * \file
 * \brief hierarchical state machine

 * \author  Nandkishor Biradar
 * \date    01 December 2018

 *  Copyright (c) 2018-2019 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

#ifndef HSM_H
#define HSM_H

#ifdef HSM_CONFIG
#include "hsm_config.h"
#endif // HSM_CONFIG

/*
 *  --------------------- DEFINITION ---------------------
 */

#ifndef HIERARCHICAL_STATES
//! Default configuration is hierarchical state machine
#define  HIERARCHICAL_STATES    1
#endif // HIERARCHICAL_STATES

#ifndef STATE_MACHINE_LOGGER
#define STATE_MACHINE_LOGGER     0        //!< Disable the logging of state machine
#endif // STATE_MACHINE_LOGGER

#ifndef HSM_USE_VARIABLE_LENGTH_ARRAY
#define HSM_USE_VARIABLE_LENGTH_ARRAY 1
#endif

/*
 *  --------------------- ENUMERATION ---------------------
 */

//! List of state machine result code
typedef enum
{
  EVENT_HANDLED,      //!< Event handled successfully.
  EVENT_UN_HANDLED,    //!< Event could not be handled.
  //!< Handler handled the Event successfully and posted new event to itself.
  TRIGGERED_TO_SELF,
}state_machine_result_t;

/*
 *  --------------------- STRUCTURE ---------------------
 */

#if HIERARCHICAL_STATES
typedef struct hierarchical_state state_t;
#else
typedef struct finite_state state_t;
#endif // HIERARCHICAL_STATES

typedef struct state_machine_t state_machine_t;
typedef state_machine_result_t (*state_handler) (state_machine_t* const State);
typedef void (*state_machine_event_logger)(uint32_t state_machine, uint32_t state, uint32_t event);
typedef void (*state_machine_result_logger)(uint32_t state, state_machine_result_t result);

//! finite state structure
struct finite_state{
  state_handler Handler;      //!< State handler function
  state_handler Entry;        //!< Entry action for state
  state_handler Exit;          //!< Exit action for state.

#if STATE_MACHINE_LOGGER
  uint32_t Id;              //!< unique identifier of state within the single state machine
#endif
};

//! Hierarchical state structure
struct hierarchical_state
{
  state_handler Handler;      //!< State handler function
  state_handler Entry;        //!< Entry action for state
  state_handler Exit;          //!< Exit action for state.

#if STATE_MACHINE_LOGGER
  uint32_t Id;              //!< unique identifier of state within the single state machine
#endif

  const state_t* const Parent;    //!< Parent state of the current state.
  const state_t* const Node;       //!< Child states of the current state.
  uint32_t Level;            //!< Hierarchy level from the top state.
};

//! Abstract state machine structure
struct state_machine_t
{
   uint32_t Event;          //!< Pending Event for state machine
   const state_t* State;    //!< State of state machine.
};

/*
 *  --------------------- EXPORTED FUNCTION ---------------------
 */

#ifdef __cplusplus
extern "C"  {
#endif // __cplusplus

extern state_machine_result_t dispatch_event(state_machine_t* const pState_Machine[],
                                            uint32_t quantity
#if STATE_MACHINE_LOGGER
                                            ,state_machine_event_logger event_logger
                                            ,state_machine_result_logger result_logger
#endif // STATE_MACHINE_LOGGER
                                            );

#if HIERARCHICAL_STATES
extern state_machine_result_t traverse_state(state_machine_t* const pState_Machine,
                                                       const state_t* pTarget_State);
#endif // HIERARCHICAL_STATES

extern state_machine_result_t switch_state(state_machine_t* const pState_Machine,
                                                    const state_t* const pTarget_State);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // HSM_H
