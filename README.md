[![Build Status](https://travis-ci.com/kiishor/UML-State-Machine-in-C.svg?branch=master)](https://travis-ci.com/kiishor/UML-State-Machine-in-C)
[![Coverage Status](https://coveralls.io/repos/github/kiishor/UML-State-Machine-in-C/badge.svg?branch=master)](https://coveralls.io/github/kiishor/UML-State-Machine-in-C?branch=master)
[![Gitter](https://badges.gitter.im/UML-State-Machine-in-C/community.svg)](https://gitter.im/UML-State-Machine-in-C/community?utm_source=badge&utm_medium=badge&utm_campaign=pr-badge)

UML State Machine in C
======================

This is a lightweight framework for UML state machine implemented in C. It supports both finite state machine and hierarchical state machine. The framework is independent of CPU, operating systems and it is developed specifically for embedded application in mind.

- The framework is very minimalistic. It has only 3 API's, 2 structures and 1 enumeration.
- It uses only **116** bytes[1] of code memory for finite state machine and **424** bytes[1] of code memory for a hierarchical state machine. It doesn't use any data memory for the framework itself.
> [1]Compiled in IAR ARM 8.30 compiler in release mode.

The framework contains three files
1. hsm.c : Implementation of framework
2. hsm.h : Contains the API's and structure declarations
3. hsm_config.h : Compile-time configuration of framework.

To read more about finite state machine and hierarchical state machine please go through the following links.
<https://en.wikipedia.org/wiki/UML_state_machine>
<https://en.wikipedia.org/wiki/Finite-state_machine>


State Machine
-------------
The State machine is represented by `state_machine_t` data type. It is an abstract structure that can be inherited to create a state machine.
The `state_machine_t` must be the first member in the derived state machine.

```C
//! Abstract state machine structure
struct state_machine_t
{
   uint32_t Event;          //!< Pending Event for state machine
   const state_t* State;    //!< State of state machine.
};
```
Example.

```C
// Derived state machine
struct user_state_machine
{
  state_machine_t;    // Base state machine. Must be the first member of user derived state machine.

  // User specific state machine members
  uint32_t param1;
  uint32_t param2;
  uint32_t param3;
};

```

### Event
 Event is represented by 32-bit unsigned int. The `Event` field in the `state_machine_t` holds the event value to pass it to the state machine. The `Event` (in `state_machine_t`) equal to zero indicates that state machine is ready to accept new event. Write any non-zero value in the `Event` to pass it to the state machine. The framework clears the Event when state machine processes it successfully. Do not write new event value, when the `Event` field in the `state_machine_t` is not zero.
In this case, you need to implement Queue in the state machine to push the new event value when state machine is still busy processing the event.


### State
State is represented by a pointer to `state_t` structure in the framework.

If framework is configured for finite state machine then `state_t` contains,

```C
typedef struct finite_state_t state_t;

// finite state structure
typedef struct finite_state_t{
  state_handler Handler;        //!< State handler function
  state_handler Entry;          //!< Entry action for state
  state_handler Exit;           //!< Exit action for state.
}finite_state_t;
```

- Handler: pointer to function to handle the events of state
- Entry: pointer to function execute entry action of state
- Exit: pointer to function to execute exit action of state

>The Handler must be initialized.
>The Entry and Exit actions are optional and can be initialized to NULL if not required.

If a framework is configured to support hierarchical state machine. It contains additional three members to represent the hierarchical relation between the states.

```C
typedef struct hierarchical_state_t state_t;

//! Hierarchical state structure
typedef struct hierarchical_state_t
{
  state_handler Handler;      //!< State handler function
  state_handler Entry;        //!< Entry action for state
  state_handler Exit;         //!< Exit action for state.

  const state_t* const Parent;    //!< Parent state of the current state.
  const state_t* const Node;      //!< Child states of the current state.
  uint32_t Level;                 //!< Hierarchy level from the top state.
}hierarchical_state_t;

```

- Parent: pointer to parent state of the current state
- Node:   pointer to child state of the current state
- Level:  Hierarchical level of state from root. Root state has level zero.

The states can be created compile-time and no additional functions from framework are required for generation of states.
In addition, they can be defined as **const** so that it stored in the read only memory.


event dispatcher
----------------
`dispatch_event` dispatches the event to the state machine.

```C
state_machine_result_t dispatch_event(state_machine_t* const pState_Machine[], uint32_t quantity);
```

The function takes the arguments `pState_Machine` as an array of state machines and number of state machines in the array. It iterates through all the state machines in the array and if any state machine has pending event, then it dispatches the event to the state machine. The framework supports the priority based event dispatching. The index of state machine in the array decides the priority of the state machine. The lower the index, higher the priority. That means if two or more state machines have pending event then the dispatcher will dispatch the event of the highest priority (lowest index in the array) state machine first.

The framework works on **run to completion** principle. Hence, once the event is dispatched it will not be preempted by higher priority state machine. After processing of event, the dispatcher starts iteration from the first (highest priority) state machine to check for pending event. The event to higher priority state machine is dispatched only after completion of current event processing.

The function returns when there is no pending event in the array of state machines or if an event is not handled successfully.

```C
//! List of state machine result code
typedef enum
{
  EVENT_HANDLED,      //!< Event handled successfully.
  EVENT_UN_HANDLED,    //!< Event could not be handled.
  //!< Handler handled the Event successfully and posted new event to itself.
  TRIGGERED_TO_SELF,
}state_machine_result_t;
```

- EVENT_HANDLED: All the pending events in the array of state machine has dispatched and handled successfully.
- EVENT_UN_HANDLED: The framework terminated as state machine could not handled the event.

> The `dispatch_event` never returns 'TRIGGERED_TO_SELF' return code.

Call this function periodically. The recommended approach is
```C
while(1)
{
  if(dispatch_event(State_Machines, 1) == EVENT_UN_HANDLED)
  {
    // log error
  }
  wait_for_event();   // Go to low power mode waiting for external event to trigger.
}
```

State transition
----------------
The framework supports two types of state transition,
1. switch_state:

```C
state_machine_result_t switch_state(state_machine_t* const pState_Machine, const state_t* pTarget_State);
```

   Use this function when framework is configured for finite state machine. You can also use this function in hierarchical state machine if the source and target states have a common parent state. It calls the exit action of source state and then calls the entry action of target state in the state transition.

2. traverse_state:

```C
state_machine_result_t traverse_state(state_machine_t* const pState_Machine, const state_t* pTarget_State);
```
   Use this function when you need to traverse through the hierarchy from source state to target state. It calls the exit action of each parent state of source while traversing from the source state. It calls the entry action of each parent state while traversing to the target state.

Configuration
-------------

The framework can be configured to support different use cases.
To change the default configuration, define a macro `HSM_CONFIG` using compiler -D option and provide a path to "hsm_config.h" file.
Add your configuration in hsm_config.h to override the default configuration in the hsm.h.
Modifying hsm.h for configuration is not recommended.

### finite/hierarchical state machine

The framework can be configured to support either **finite state machine** or **hierarchical state machine** using a macro in hsm_config.h file.
By default, the framework supports hierarchical state machine. Use "hsm_config.h" file to customize the framework.

Change the value of `HIERARCHICAL_STATES` (in the "hsm_config.h") to enable/disable **hierarchical state machine**.

```C
// 0 : disable hierarchical state machine. Only Finite state machine is supported.
// 1 : enable Hierarchical state machine. Both finite and hierarchical state machine is supported.
#define  HIERARCHICAL_STATES      0
```

### Enable logging

Change the value of `STATE_MACHINE_LOGGER` to enable/disable state machine logging.
By default, logging is disabled.

```C
// 0: disable the state machine logger
// 1: enable the state machine logger
#define STATE_MACHINE_LOGGER     1
```

### Disable Variable length array

The `traverse_state` function uses variable length array feature in the implementaion of hierarchical state machine.
If you want to disable variable length array used in the framework,
either due to compiler doesn't support or some other reason,
then `HSM_USE_VARIABLE_LENGTH_ARRAY` to 0.

If you disable "variable length array" then you need to manually provide the highest value of hierarchical level among all the state machines.
use `MAX_HIERARCHICAL_LEVEL` to set the maximum hierarchical level. This should be highest `state_t::Level` value among all the state machines.

By default, framework uses variable length array implementation.

```C
// 0: disable variable length array used in hsm.c
// 1: enable variable length aray used in hsm.c
#define HSM_USE_VARIABLE_LENGTH_ARRAY 1
```

State machine logging
---------------------

The framework supports the logging mechanism for debugging purpose using two callback functions.
User needs to implement logger functions and pass it to `dispatch_event`.

```C
typedef void (*state_machine_event_logger)(uint32_t state_machine, uint32_t state, uint32_t event);
typedef void (*state_machine_result_logger)(uint32_t state, state_machine_result_t result);
```
### state_machine_event_logger
This function is called before dispatching the event to state machine. The framework passes 3 arguments to this callback function.

- state_machine: index of state machine in the array
- state: unique id of current state in the state machine.
- event: event to be dispatched to the state machine.

### state_machine_result_logger
This function is called after dispatching the event to state machine. The framework passes 2 arguments to this callback function.

- state: unique id of the current state after handling of the event.
- result: Result of event handled by state machine.

Users can use this logging mechanism to also log the time consumed to handle the event by state machine.
Start timer on `state_machine_event_logger` and stop on `state_machine_result_logger`.

### Demo
[simple state machine](demo/simple_state_machine/readme.md)  
[simple state machine (enhanced)](demo/simple_state_machine_enhanced/readme.md)  
[simple hierarchical state machine](demo/hierarchical_state_machine/readme.md)  
[Toaster oven](demo/toaster_oven/readme.md)  

### Todo
- Add more real world examples/demo's specially related to embedded systems
- Add support of CMake so that different types of IDE and compilers can be supported in the demo's
- Add support of continuous integration

### More
- For reporting issues/bugs or requesting features use GitHub issue tracker
- for discussion, questions or feedback use
   https://groups.google.com/forum/#!forum/minimalist-uml-state-machine-in-c

