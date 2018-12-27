/**
 * \file
 * \brief Simple finite state machine example

 * \author	Nandkishor Biradar
 * \date		14 December 2018

 *	Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE.txt or copy at https://mit-license.org/)
 */

/*
 *	--------------------- ENUMERATION ---------------------
 */

typedef enum
{
	IDLE_STATE,
	ACTIVE_STATE,
	PAUSE_STATE
}process_state_t;

typedef enum
{
	START = 's',
	START_CAPS = 'S',

	QUIT = 'q',
	QUIT_CAPS = 'Q',

	PAUSE = 'p',
	PAUSE_CAPS = 'P',

	RESUME = 'r',
	RESUME_CAPS = 'R',

	TIMEOUT = 't',
	TIMEOUT_CAPS = 'T',

	GET_TIME = 'g',
	GET_TIME_CAPS = 'G',

}process_event_t;

/*
 *	--------------------- ENUMERATION ---------------------
 */

typedef struct
{
	state_machine_t;
  uint32_t Set_Time;
  uint32_t Resume_Time;
  uint32_t Timer;
}process_t;

/*
 *	--------------------- External function prototype ---------------------
 */

void init_process(process_t * const pProcess, uint32_t processTime);
