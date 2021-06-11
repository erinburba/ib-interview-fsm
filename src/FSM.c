/**
 * Erin Burba
 * Impact Biosystems: Firmware Interview
 * FSM.c
 * 
 * Source file for my implementation of the Finite State Machine
 * described here: https://www.notion.so/Finite-State-Machine-3b9a2982f4344f1c997372f579a02bd7
 */


#include "FSM.h"
#include <unistd.h>
#include <stdio.h>


/**
 * DeviceState -> EventHandler
 * state (DeviceState): the identifying state
 * 
 * Helper function.
 * Given a DeviceState, return a pointer to the associated EventHandler function.
 */
EventHandler GetSteadyStateFunc(DeviceState state) {
    switch (state) {
        case STATE_NORMAL:
            return *StateNormalHandler;
        case STATE_SILENT:
            return *StateSilentHandler;
        case STATE_OVER_TEMP:
            return *StateOverTempHandler;
        case STATE_NO_COVER:
            return *StateNoCoverHandler;
        default:
            printf("Invalid input: non-steady state.\n");
            exit(-1);
    }
}


/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Initialize the main logic board.
 */
DeviceState StateInitMainHandler(DeviceState previousState) {
    printf("Initialize Main Board: Attempt to initialize the main board logic.\n");

    MainBoardInit();

    return STATE_INIT_MAIN;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Board booting failed; waiting for retry.
 */
DeviceState StateHaltHandler(DeviceState previousState) {
    printf("Halted: Failed to initialize the board. Retrying...\n");

    return STATE_HALT;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): the previous state; used if it's a steady state
 * Return the device's new state.
 * 
 * Board initialized; if passed a steady state as previousState, run again in
 * that state. Otherwise, attempt to initialize the device's outputs.
 */
DeviceState StateInitHandler(DeviceState previousState) {
    if (previousState && previousState != STATE_INIT_MAIN) {
        printf("Initialized: Resuming steady state operation.\n");
        return (*GetSteadyStateFunc(previousState)) (STATE_INIT);
    }

    printf("Initialized: Booted and ready for sensor/user inputs and device outputs.\n");
    OutputsInit();

    return STATE_INIT;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Normal operation: fans on, get user input and use it to set the output.
 */
DeviceState StateNormalHandler(DeviceState previousState) {
    printf("Normal: Checking sensors, reading user inputs, regular outputs.\n");

    FansOn();
    SetOutput(GetUserInput());

    return STATE_NORMAL;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Silent operation: turn the fans off, and set the output to a reduced level.
 */
DeviceState StateSilentHandler(DeviceState previousState) {
    printf("Silent: Fans disabled, reduced outputs.\n");

    FansOff();
    SetOutput(REDUCED_OUTPUT);
    
    return STATE_SILENT;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Error state: turn the fans on, and the the output to a reduced level.
 */
DeviceState StateOverTempHandler(DeviceState previousState) {
    printf("Over Temp: Error! Temperature above threshold, reducing outputs until cooled.\n");
    
    FansOn();
    SetOutput(REDUCED_OUTPUT);

    return STATE_OVER_TEMP;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Temperature error mitigated; resume operating in state prior to the error.
 */
DeviceState StateNormalTempHandler(DeviceState previousState) {
    return (*GetSteadyStateFunc(previousState)) (STATE_OVER_TEMP);
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored
 * Return the device's new state.
 * 
 * Error state: turn the fans off, and turn off the device's output.
 */
DeviceState StateNoCoverHandler(DeviceState previousState) {
    printf("No Cover: Error! Fans off and outputs disabled until the cover is replaced.\n");

    FansOff();
    SetOutput(NO_OUTPUT);

    return STATE_NO_COVER;
}

// Array of all state/event/handler combinations in the FSM.
struct StateMachine FSM [N_STATE_TRANSITIONS];

FSM = {
    {STATE_POR, EVENT_POR_DONE, StateInitMainHandler},
    {STATE_INIT_MAIN, EVENT_BOARD_BOOT_FAIL, StateHaltHandler},
    {STATE_INIT_MAIN, EVENT_BOARD_BOOT_SUCCESS, StateInitHandler},
    {STATE_HALT, EVENT_BOARD_BOOT_RETRY, StateInitMainHandler},
    {STATE_INIT, EVENT_BOOT_SUCCESS, StateNormalHandler},
    {STATE_INIT, EVENT_COVER_OFF, StateNoCoverHandler},
    {STATE_INIT, EVENT_TEMPERATURE_HIGH, StateOverTempHandler},
    {STATE_INIT, EVENT_SILENT_BUTTON, StateSilentHandler},
    {STATE_NORMAL, EVENT_USER_INPUT, StateNormalHandler},
    {STATE_NORMAL, EVENT_COVER_OFF, StateNoCoverHandler},
    {STATE_NORMAL, EVENT_TEMPERATURE_HIGH, StateOverTempHandler},
    {STATE_NORMAL, EVENT_SILENT_BUTTON, StateSilentHandler},
    {STATE_SILENT, EVENT_COVER_OFF, StateNoCoverHandler},
    {STATE_SILENT, EVENT_TEMPERATURE_HIGH, StateOverTempHandler},
    {STATE_SILENT, EVENT_SILENT_BUTTON, StateNormalHandler},
    {STATE_OVER_TEMP, EVENT_COVER_OFF, StateNoCoverHandler},
    {STATE_OVER_TEMP, EVENT_TEMPERATURE_NORMAL, StateNormalTempHandler},
    {STATE_NO_COVER, EVENT_COVER_ON, StateInitHandler},
    {STATE_NORMAL, EVENT_NOISY_BUTTON, StateSilentHandler}
};

/**
 * Main function to run device's FSM.
 * 
 * Runs in an infinite loop: waits until an event occurs, then
 * uses that event to transition state when possible.
 */
int main(int argc, char *argv[]) {
    DeviceState currentState = STATE_POR;
    DeviceState previousState = NULL, nextState = NULL;

    while (1) {
        // Wait for an event to happen on the device.
        DeviceEvent event = GetEvent();

        // If the event causes a transition from the current state,
        // call the handler to get the next state.
        if (FSM[currentState][event] != NULL) {
            nextState = (*FSM[currentState][event])(previousState);
            previousState = currentState;
            currentState = nextState;
        }

    }
}
