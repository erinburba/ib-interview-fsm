# Code Overview
A description of code design, theory of operation, assumptions, and instructions for expansion.

## Design Overview
All states are listed in the DeviceState enum. All events that would cause a state transition or update are listed in the DeviceEvent enum. States that need to handle specific events to transition to another state must have a corresponding EventHandler function (DeviceState -> DeviceState) that defines the new state of the device after the event occurred in that state. These sets of state, event, and event handler are collected in a struct called StateMachine.

The finite state machine can be described in its entirety by listing all valid StateMachine combinations. The main control loop uses the list of all StateMachines to progress through the program. In the main control loop, events are consumed as they occur and combined with the current state to cause a transition when possible.

## Theory of Operation
This design relies heavily, perhaps generously, on the use of interrupts or events. Interrupts are a favorable method of handling events that happen within a system because they generally only use significant computational resources when something happens. This is a great improvement over polling, which periodically checks and updates the system regardless of any changes. In this instance, it means that the main control loop doesn't have to repeatedly poll sensors and inputs, and instead waits until there is something requiring attention.

If the device's architecture doesn't support interrupts or events through background processes, the design could be easily modified to use other methods such as polling.

## Extensibility
A key benefit of this design is that it is easily extensible to support new states and events. New states and events can be freely added to the enums, and adding transitions is as simple as grouping a state, event, and handler function together in a StateMachine struct. To add a new state transition to the finite state machine, a developer must:
1. Add any new states and events to the enums in FSM.h.
2. Write an EventHandler function (DeviceState -> DeviceState) to control the device and produce the new state that results from the transition.
3. Add the state/event/handler struct to the StateMachine array FSM in FSM.c, and increment N_STATE_TRANSITIONS in FSM.h.


## Assumptions/Decisions
* The silent mode toggle button does nothing if the device is in an error state.
* The device's architecture supports system interrupts. If they aren't supported, polling mechanisms can be used to read sensors and detect events.
* Only 3/8 states use the previous state to decide future states; I have it as a param in every state function because the consistent function signature simplifies the StateMachine struct.