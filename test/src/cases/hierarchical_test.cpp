/**
 * \file
 * \brief Event handling of hierarchical state machine
 * \author	Nandkishor Biradar
 * \date	20 December 2018

 *	Copyright (c) 2018 Nandkishor Biradar
 *  https://github.com/kiishor

 *  Distributed under the MIT License, (See accompanying
 *  file LICENSE.txt or copy at https://mit-license.org/)
 */
 
#include "catch.hpp"
#include "hsm.h"
#include "gmock/gmock.h"
#include "gmock-global.h"

using ::testing::Return;
using ::testing::InSequence;
using ::testing::Invoke;

namespace hierarchical_test
{

MOCK_GLOBAL_FUNC1(handler1, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler2, state_machine_result_t(state_machine_t * const));
MOCK_GLOBAL_FUNC1(handler3, state_machine_result_t(state_machine_t * const));

extern const hierarchical_state_t childHSM[1];
const hierarchical_state_t rootHSM =
{
	handler1,
	NULL,
	NULL,
	NULL,
	childHSM,
	0
};

const hierarchical_state_t childHSM[1] =
{
	{
		handler2,
		NULL,
		NULL,
		&rootHSM,
		NULL,
		1
	}
};

const hierarchical_state_t child1HSM[1] =
{
	{
		handler3,
		NULL,
		NULL,
		childHSM,
		NULL,
		1
	}
};

SCENARIO("Composite state machine test")
{


typedef enum
{
	EN_EVENT1 = 1,
	EN_EVENT2,
	EN_EVENT3,
	EN_EVENT4
}en_event;

	GIVEN( "A composite state machine" )
	{
		state_machine_t machine;
		state_machine_t * const machineList[] = {&machine};
		machine.State = child1HSM;

		WHEN("handler couldn't handle the event")
		{
			machine.Event = EN_EVENT1;

			EXPECT_GLOBAL_CALL(handler3, handler3(&machine))
			.WillOnce(Return(EVENT_UN_HANDLED));
			EXPECT_GLOBAL_CALL(handler2, handler2(&machine))
			.WillOnce(Return(EVENT_UN_HANDLED));
			EXPECT_GLOBAL_CALL(handler1, handler1(&machine));

			THEN( "Machine invokes the parent handler")
			{
				REQUIRE(dispatch_event(machineList, 1) == EVENT_HANDLED);
				REQUIRE(machine.Event == 0);
			}
		}
	}
}

}


