#include "FSM.h"
#include <unistd.h>
#include <stdio.h>


/**
 * None -> DeviceState or NULL
 * Return the error DeviceState if one is detected.
 * 
 * Checks for missing cover and high temperature, and returns an error
 * DeviceState if either is detected. Returns NULL if there is no error.
 * The cover is checked first; in the case that both errors are present in
 * the device, the cover error will take precedence and lead to STATE_NO_COVER.
 */
DeviceState ErrorDetection() {
    if (!IsCoverPresent()) {
        return STATE_NO_COVER;
    }

    if (GetTemperatureC() > MAX_TEMP) {
        return STATE_OVER_TEMP;
    }

    return NULL;
}

/**
 * None -> DeviceState
 * Return the correct device state
 * 
 * Return the state of the device based on a boolean value
 * that changes through interrupt handling.
 */
DeviceState GetSteadyState() {
    if (isSilent) {
        return STATE_SILENT;
    } else {
        return STATE_NORMAL;
    }
}

/**
 * None -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Power-On Reset state
 */
DeviceState StatePORHandler(DeviceState previousState) {
    printf("Power-On Reset: Device powered on.\n");

    return STATE_INIT_MAIN;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Main logic board initialization state. Return true if board initialization
 * succeeds, or false if it fails.
 */
DeviceState StateInitMainHandler(DeviceState previousState) {
    printf("Initialize Main Board: Attempt ot initialize the main board logic.\n");

    if (MainBoardInit()) {
        return STATE_INIT;
    }

    return STATE_HALT;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Halted (failed boot) state. Always return STATE_INIT_MAIN to retry initialization.
 */
DeviceState StateHaltHandler(DeviceState previousState) {
    printf("Halted: Failed to initialize the board. Retrying...\n");

    return STATE_INIT_MAIN;
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): if non-null, the following state of the device.
 * Return the device's next state.
 * 
 * Board booted, initializing outputs state. Check on the state of sensors and
 * user inputs to determine which steady state the device should enter next.
 * If previousState is not NULL, that state overrides any sensor/input checks and
 * is returned as the device's next state.
 */
DeviceState StateInitHandler(DeviceState previousState) {
    

    if (previousState) {
        printf("Initialized: Resuming steady state operation.\n");
        return previousState;
    }

    printf("Initialized: Booted and ready for sensor/user inputs and device outputs.\n");

    OutputsInit();

    DeviceState errorState = ErrorDetection();
    if (errorState) {
        return errorState;
    } else {
        return GetSteadyState();
    }   
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Normal operation state: check sensors and user inputs, adjust outputs
 */
DeviceState StateNormalHandler(DeviceState previousState) {
    if (previousState != STATE_NORMAL) {
        printf("Normal: Checking sensors, reading user inputs, regular outputs.\n");
    }

    DeviceState errorState = ErrorDetection();
    if (errorState) {
        return errorState;
    }

    FansOn();
    SetOutput(GetUserInput());

    return GetSteadyState();
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Steady adjusted state: disable fans, scale back outputs
 */
DeviceState StateSilentHandler(DeviceState previousState) {
    if (previousState != STATE_SILENT) {
        printf("Silent: Fans disabled, reduced outputs.\n");
    }

    DeviceState errorState = ErrorDetection();
    if (errorState) {
        return errorState;
    }

    FansOff();
    SetOutput(REDUCED_OUTPUT);
    
    return GetSteadyState();
}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): the device's previous state, to which it will 
 *                              return after the temperature regulates.
 * Return the device's next state.
 * 
 * Error state: reduce outputs and stay in the state until the temperature regulates.
 */
DeviceState StateOverTempHandler(DeviceState previousState) {
    printf("Over Temp: Error! Temperature above threshold, reducing outputs until cooled.\n");

    SetOutput(REDUCED_OUTPUT);
    FansOn();

    while (1) {
        if (!IsCoverPresent()) {
            return STATE_NO_COVER;
        } else if (GetTemperatureC() <= MAX_TEMP) {
            return previousState;
        }
        
        sleep(PERIOD_S);
    }

}

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): the device's previous state, to which it will 
 *                              return after the cover is replaced.
 * Return the device's next state.
 * 
 * Error state: disable outputs and stay in this state until the cover is replaced.
 */
DeviceState StateNoCoverHandler(DeviceState previousState) {
    printf("No Cover: Error! Fans off and outputs disabled until the cover is replaced.\n");

    FansOff();
    SetOutput(NO_OUTPUT);

    while (1) {
        if (IsCoverPresent()) {
            return previousState;
        }
        
        sleep(PERIOD_S);
    }
}


/**
 * DeviceState -> StateFunc
 * state (DeviceState): the device's operating state
 * Return the function pointer StateFunc associated with the 
 * given DeviceState.
 * 
 * Given a valid DeviceState, return a pointer to the function 
 * that handles the device in that state. Print an error message
 * and exit if the given state isn't recognized.
 */
StateFunc getStateFunc(DeviceState state) {

    // Return the correct function pointer for a state handling function.
    switch(state) {
        case STATE_POR:
                return &StatePORHandler;
        case STATE_INIT:
                return &StateInitHandler;
        case STATE_HALT:
                return &StateHaltHandler;
        case STATE_INIT_MAIN:
                return &StateInitMainHandler;
        case STATE_NORMAL:
                return &StateNormalHandler;
        case STATE_SILENT:
                return &StateSilentHandler;
        case STATE_NO_COVER:
                return &StateNoCoverHandler;
        case STATE_OVER_TEMP:
                return &StateOverTempHandler;
        default:
                // Should never be reached as long as every state 
                // has a corresponding handler function.
                printf("Invalid state! Erin what happened!\n");
                printf(state);
                exit(-1);
    }

}

/**
 * Main function to run device's FSM.
 * 
 * Runs in an infinite loop, updating the device's state in response
 * to sensors and events within the state handler functions.
 */
int main(int argc, char *argv[])
{
    // Start the devivce in normal (non-silent) operation.
    isSilent = false;
    DeviceState nextState, previousState, currentState = STATE_POR;

    while (1) {

        // Execute the device's operations in the current state, and get 
        // the next state.
        nextState = (*getStateFunc(currentState))(currentState);
        previousState = currentState;
        currentState = nextState;

        sleep(PERIOD_S);
    }
}
