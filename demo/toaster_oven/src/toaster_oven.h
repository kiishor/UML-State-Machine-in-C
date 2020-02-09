#ifndef TOASTER_OVEN_H
#define TOASTER_OVEN_H

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
 *  --------------------- ENUMERATION ---------------------
 */

//! List of oven events
typedef enum
{
  EN_START = 1,
  EN_STOP,
  EN_DOOR_OPEN,
  EN_DOOR_CLOSE,
  EN_TIMEOUT,
}oven_event_t;

typedef enum
{
  DOOR_OPENED,
  DOOR_CLOSED
}door_status_t;

/*
 *  --------------------- STRUCTURE ---------------------
 */

//! Oven state machine
typedef struct
{
  state_machine_t Machine;  //!< Abstract state machine
  uint32_t Set_Time;        //!< Set time of a oven
  uint32_t Resume_Time;     //!< Remaining time when the oven is paused
  uint32_t Timer;           //!< Oven timer
  bool Lamp;                //!< Oven lamp
  bool Heater;              //!< Oven heater
}oven_t;

/*
 *  --------------------- External function prototype ---------------------
 */

extern void init_oven(oven_t* const pOven, uint32_t toastTime, door_status_t status);

/*
 *  --------------------- Inline functions ---------------------
 */

 // Toaster oven APIs

static inline void start_oven(oven_t* const pOven)
{
  pOven->Machine.Event = EN_START;
}

static inline void stop_oven(oven_t* const pOven)
{
  pOven->Machine.Event = EN_STOP;
}

static inline void open_door(oven_t* const pOven)
{
  pOven->Machine.Event = EN_DOOR_OPEN;
}

static inline void close_door(oven_t* const pOven)
{
  pOven->Machine.Event = EN_DOOR_CLOSE;
}

static inline void on_oven_timedout(oven_t* const pOven)
{
  pOven->Machine.Event = EN_TIMEOUT;
}

/** \brief Parses the user keyboard input and calls the respective API,
 *  to trigger the events to state machine.
 *
 * \param pOven oven_t* const: instance of oven_t state machine.
 * \param input char:  user input
 *
 */
static inline void parse_cli(oven_t* const pOven, char input)
{
  switch(input)
  {
  case 's':
  case 'S':
    start_oven(pOven);
    break;

  case 'q':
  case 'Q':
    stop_oven(pOven);
    break;

  case 'o':
  case 'O':
    open_door(pOven);
    break;

  case 'c':
  case 'C':
    close_door(pOven);
    break;

  default:
    printf("Not a valid event\n");
    break;
  }
}

#endif // TOASTER_OVEN_H
