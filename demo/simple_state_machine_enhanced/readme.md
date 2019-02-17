Simple state machine (Enhanced)
===============================

This is an enhanced version of simple state machine demo
[simple state machine](../simple_state_machine/readme.md)

This demo uses macro to enumerate the list of states and to initialize the state_t in process.c

```C
#define ALL_PROCESS_STATE \
 ADD_STATE(IDLE_STATE, idle_handler, idle_entry_handler, idle_exit_handler)  \
 ADD_STATE(ACTIVE_STATE, active_handler, active_entry_handler, active_exit_handler)  \
 ADD_STATE(PAUSE_STATE, paused_handler, paused_entry_handler, paused_exit_handler)
```

```C
//! List of states in the process state machine
#define ADD_STATE(name, ...)  name,
typedef enum
{
  ALL_PROCESS_STATE
}process_state_t;
#undef ADD_STATE
```
```C
#define ADD_STATE(name, state_handler, entry_handler, exit_handler) \
[name].Handler = state_handler, \
[name].Entry   = entry_handler, \
[name].Exit    = exit_handler,  \
[name].Id      = name,

static const state_t Process_States[] =
{
  ALL_PROCESS_STATE
};

#undef ADD_STATE
```
