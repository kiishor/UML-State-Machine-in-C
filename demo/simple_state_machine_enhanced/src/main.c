/**
 * \file
 * \brief   Simple state machine example

 * \author  Nandkishor Biradar
 * \date    14 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
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

#include "hsm.h"
#include "finite_state_machine.h"

/*
 *  --------------------- Global variables ---------------------
 */
process_t SampleProcess;

state_machine_t * const State_Machines[] = {(state_machine_t *)&SampleProcess};

sem_t Semaphore;


/*
 *  --------------------- Functions ---------------------
 */

void event_logger(uint32_t stateMachine, uint32_t state, uint32_t event)
{
  printf("State Machine: %d, State: %d, Event: %d\n", stateMachine, state, event);
}

void result_logger(uint32_t state, state_machine_result_t result)
{
  printf("Result: %d, New State: %d\n", result, state);
}

void* timer(void* vargp)
{
    while(1)
    {
      sleep(1);
      if(SampleProcess.Timer > 0)
      {
        SampleProcess.Timer--;
        if(SampleProcess.Timer == 0)
        {
          on_process_timedout(&SampleProcess);
          sem_post(&Semaphore);
        }
      }
    }
    return NULL;
}

void* console(void* vargp)
{
  while(1)
  {
    // Get input from console
    char input  = getchar();

    // ignore new line input
    if((input == '\n' ) ||(input == '\r'))
    {
      continue;
    }

    parse_cli(&SampleProcess, input);
    sem_post(&Semaphore);
  }
}

int main(void)
{
  init_process(&SampleProcess, 10);

  pthread_t timer_thread, console_thread;
  pthread_create(&timer_thread, NULL, timer, NULL);
  pthread_create(&console_thread, NULL, console, NULL);
  sem_init(&Semaphore, 0, 1);

  while(1)
  {
    sem_wait(&Semaphore);
    if(dispatch_event(State_Machines, 1, event_logger, result_logger) == EVENT_UN_HANDLED)
    {
      printf("invalid event entered\n");
    }
  }
  return 0;
}
