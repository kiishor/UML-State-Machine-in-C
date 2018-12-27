/**
 * \file
 * \brief   Simple state machine example

 * \author  Nandkishor Biradar
 * \date    14 December 2018

 *  Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE.txt or copy at https://mit-license.org/)
 */

#include <stdint.h>
#include <stdio.h>

#include "hsm.h"
#include "finite_state_machine.h"


process_t SampleProcess;

state_machine_t * const State_Machines[] = {(state_machine_t *)&SampleProcess};


void event_logger(uint32_t stateMachine, uint32_t state, uint32_t event)
{
  printf("State Machine: %d, State: %d, Event: %c\n", stateMachine, state, event);
}

void result_logger(uint32_t state, state_machine_result_t result)
{
  printf("Result: %d, New State: %d\n", result, state);
}
int main(void)
{
  init_process(&SampleProcess, 10);
  while(1)
  {

    // Get event from console
    SampleProcess.Event  = getchar();

    // ignore new line input
    if((SampleProcess.Event == '\n' ) ||(SampleProcess.Event == '\r'))
    {
      continue;
    }


    if(dispatch_event(State_Machines, 1, event_logger, result_logger) == EVENT_UN_HANDLED)
    {
      printf("invalid event entered\n");
    }
  }
}
