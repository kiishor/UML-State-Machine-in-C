/**
 * \file
 * \brief   toaster oven - Hierarchical state machine example

 * \author  Nandkishor Biradar
 * \date    01 February 2020

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
#include <pthread.h>
#include <unistd.h>
#include <semaphore.h>
#include <stdbool.h>

#include "hsm.h"
#include "toaster_oven.h"

/*
 *  --------------------- Global variables ---------------------
 */

//! Instance of oven_t
oven_t SampleOven;

//! Create and initialize the array of state machines.
state_machine_t * const State_Machines[] = {(state_machine_t *)&SampleOven};

//! Semaphore for event synchronization timer, console and main thread.
sem_t Semaphore;


/*
 *  --------------------- Functions ---------------------
 */

/** \brief Simulate the timer ISR.
 *
 * This is an one second timer. When Oven is ON it prints the remaining time on console.
 * It also generates the timeout event when on time expires.
 */
void* timer(void* vargp)
{
  (void)(vargp);
    while(1)
    {
      sleep(1);

      if(SampleOven.Timer > 0)
      {
        SampleOven.Timer--;

        printf("\rRemaining ON time: %d ", SampleOven.Timer);

        if(SampleOven.Timer == 0)
        {
          printf("\n");
          on_oven_timedout(&SampleOven);  // Generate the timeout event
          sem_post(&Semaphore);   // signal to main thread
        }
      }
    }
    return NULL;
}

/** \brief Simulate the user inputs.
 *
 * It waits for the user key (ascii) input from console and pass it to parse_cli
 * to convert it into oven_event_t events. It supports start, stop, door open and door close events.
 */
void* console(void* vargp)
{
  (void)(vargp);
  while(1)
  {
    // Get input from console
    char input  = getchar();

    // ignore new line input
    if((input == '\n' ) ||(input == '\r'))
    {
      continue;
    }

    parse_cli(&SampleOven, input);
    sem_post(&Semaphore);
  }
}

int main(void)
{
  // Initialize the oven state machine.
  init_oven(&SampleOven, 10, DOOR_CLOSED);

  // Create timer and console thread
  pthread_t timer_thread, console_thread;
  pthread_create(&timer_thread, NULL, timer, NULL);
  pthread_create(&console_thread, NULL, console, NULL);
  sem_init(&Semaphore, 0, 1);

  while(1)
  {
    sem_wait(&Semaphore);   // Wait for event

    if(dispatch_event(State_Machines, 1) == EVENT_UN_HANDLED)
    {
      printf("invalid event entered\n");
    }
  }
  return 0;
}
