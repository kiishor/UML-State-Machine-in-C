#ifndef DEMO_H
#define DEMO_H

/**
 * \file
 * \brief hierarchical state machine demo

 * \author  Nandkishor Biradar
 * \date    03 February 2020

 *  Copyright (c) 2018-2020 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE or copy at https://mit-license.org/)
 */

/*
 *  --------------------- ENUMERATION ---------------------
 */

//! List of supported events
typedef enum
{
  EVENT1 = 1,
  EVENT2,
  EVENT3
}demo_event_t;

/*
 *  --------------------- STRUCTURE ---------------------
 */

//! Simple hierarchical state machine
typedef struct
{
  state_machine_t Machine;  //!< Abstract state machine
  uint32_t Dummy1;          //!< Dummy variable
  uint32_t Dummy2;          //!< Dummy variable
}demo_state_machine_t;

/*
 *  --------------------- External function prototype ---------------------
 */

extern void init_demo(demo_state_machine_t* const pDemo);

/*
 *  --------------------- Inline functions ---------------------
 */

/** \brief Parses the user keyboard input and calls the respective API,
 *  to trigger the events to state machine.
 *
 * \param pDemo demo_state_machine_t* const: instance of \ref demo_state_machine_t state machine.
 * \param input char:  user input
 *
 */
static inline void parse_cli(demo_state_machine_t* const pDemo, char input)
{
  switch(input)
  {
  case '1':
    pDemo->Machine.Event = EVENT1;
    break;

  case '2':
    pDemo->Machine.Event = EVENT2;
    break;

  case '3':
    pDemo->Machine.Event = EVENT3;
    break;

  default:
    printf("Not a valid event\n");
    break;
  }
}

#endif // DEMO_H
