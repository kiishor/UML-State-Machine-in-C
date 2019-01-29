#ifndef PROCESS_H
#define PROCESS_H

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
 *  --------------------- ENUMERATION ---------------------
 */

//! List of process events
typedef enum
{
  START = 1,
  STOP,
  PAUSE,
  RESUME,
  TIMEOUT,
}process_event_t;

/*
 *  --------------------- STRUCTURE ---------------------
 */

//! process state machine
typedef struct
{
  state_machine_t;      //!< Abstract state machine
  uint32_t Set_Time;    //! Set time of a process
  uint32_t Resume_Time; //!< Remaining time when the process is paused
  uint32_t Timer;       //!< Process timer
}process_t;

/*
 *  --------------------- External function prototype ---------------------
 */

extern void init_process(process_t* const pProcess, uint32_t processTime);

static inline void start_process(process_t* const pProcess)
{
  pProcess->Event = START;
}

static inline void stop_process(process_t* const pProcess)
{
  pProcess->Event = STOP;
}

static inline void pause_process(process_t* const pProcess)
{
  pProcess->Event = PAUSE;
}

static inline void resume_process(process_t* const pProcess)
{
  pProcess->Event = RESUME;
}

static inline void on_process_timedout(process_t* const pProcess)
{
  pProcess->Event = TIMEOUT;
}

static inline void get_process_remaining_time(process_t * const pProcess)
{
  printf("Remaining time: %d seconds\n", pProcess->Timer);
}

static inline void parse_cli(process_t* const pProcess, char input)
{
  switch(input)
  {
  case 's':
  case 'S':
    start_process(pProcess);
    break;

  case 'q':
  case 'Q':
    stop_process(pProcess);
    break;

  case 'p':
  case 'P':
    pause_process(pProcess);
    break;

  case 'r':
  case 'R':
    resume_process(pProcess);
    break;

  default:
    printf("Not a valid event\n");
    break;
  }
}

#endif // PROCESS_H
