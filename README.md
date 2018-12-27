UML-State-Machine-in-C
======================

This is a lightweight framework for UML state machine implemented in C. This framework supports finite state machine and hierarchical state machine. The framework is independent of CPU, operating systems and it is developed specifically for embedded application in mind.

To read more about finite state machine and hierarchical state machine please go through the following links.
<https://en.wikipedia.org/wiki/UML_state_machine>

<https://en.wikipedia.org/wiki/Finite-state_machine>


The framework contains three files
1. hsm.c : Implementation of framework
2. hsm.h : Contains the API's and structure declarations 
3. hsm_config.h : Compile-time configuration of framework.


The framework can be configured to support either finite state machine or hierarchical state machine using a macro in hsm_config.h file.

State Machine
-------------
The State machine is represented by `state_machine_t` data type. It is an abstract structure that can inherited to create a state machine.
The `state_machine_t` must be the first member of an inherited state machine.

The `state_machine_t` contains following two members,

### Event
 Event is represented by 32-bit unsigned int. The `Event` field in the `state_machine_t` holds the event value to pass it to the state machine. The `Event` (in `state_machine_t`) equal to zero indicates that state machine is ready to accept new event. Write any non-zero value in the `Event` to pass it to the state machine. The framework clears the Event when state machine processes it successfully. Do not write new event value, when the `Event` field in the `state_machine_t` is not zero.
In this case you need to implement Queue in the state machine to push the new event value when state machine is still busy in processing the event. 


### State
State is represented by pointer to `state_t` structure in the framework. 

If framework is configured for finite state machine then `state_t` contains,
- Handler: pointer to function to handle the events of state
- Entry: pointer to function execute entry action of state
- Exit: pointer to function to execute exit action of state

>The Handler must be initialized. 
>The Entry and Exit actions are optional and can be initialized to NULL if not required. 

The hierarchical state machine contains additional members to represent the hierarchical relation between the states.
- Parent: pointer to parent state of the current state 
- Node:   pointer to child state of the current state


The states can be created compile-time and no additional functions from framework is required for generation of states.
In addition they can be defined as **const** so that it stored in the read only memory.


event dispatcher
----------------
`dispatch_event` function dispatches the event to the state machine. The function takes the argument as array of state machines and quantity of state machine to process. It iterates through all the state machines in the array and if any state machine has pending event, then it dispatches the event to the state machine. The framework supports the priority based event dispatching. The index of state machine in the array decides the priority of the state machine. The lower the index, higher the priority. That means if two or more state machines have pending event then the dispatcher will dispatch the event of the highest priority (lowest index in the array) state machine first. 

The framework works on **run to completion** principle. Hence, once the event is dispatched it will not be preempted by higher priority state machine. After processing of event, the dispatcher starts iteration from the first (highest priority) state machine to check for pending event. The event to higher priority state machine is dispatched only after completion of current event processing. 

State transition
----------------
The framework supports two types of state transition,
1. switch_state: Use this function when framework is configured for finite state machine. You can also use this function in hierarchical state machine if the source state and target state have same parent state. It calls the exit action of source state and then calls the entry action of target state in the state transition.

2. traverse_state: Use this function when you need to traverse through the hierarchy from source state to target state. It calls the exit action of each parent state of source while traversing from the source state. It calls the entry action of each parent state while traversing to the target state.


Examples
--------
finite state machine

