#include <stdbool.h>

/******  VARIABLES  ******/

// Boolean that's true when the device is in silent mode.
// Toggled through interrupt handling (or at least the idea thereof).
bool isSilent;

/******  CONSTANTS  ******/

// Threshold Celsius temperature to define overheating.
#define MAX_TEMP 80

// Unit-less 0-100 output level that results in no output.
#define NO_OUTPUT 0

// Unit-less 0-100 output level value for reduced/silent operation.
#define REDUCED_OUTPUT 40

// Cycle period in seconds
#define PERIOD_S 1

/******  TYPEDEFS  ******/

// Different states of the device
typedef enum {
    STATE_POR,
    STATE_HALT,
    STATE_INIT_MAIN,
    STATE_INIT,
    STATE_NORMAL,
    STATE_SILENT,
    STATE_OVER_TEMP,
    STATE_NO_COVER
} DeviceState;

// Function pointer defining state execution functions.
typedef DeviceState(*StateFunc)(DeviceState);

/******  FUNCTION DECLARATIONS  ******/

/***  PSEUDOCODE PLACEHOLDERS  ***/

/**  INTERRUPT HANDLING FUNCTION  **/

/**
 * int -> None
 * 
 * Pseudocode/placeholder.
 * Interrupt handling function to toggle between silent
 * and normal operation in response to a button press.
 */
void HandleSilentModeButtonPress(int irq);


/**  INPUT/SENSING FUNCTIONS  **/

/**
 * None -> int
 * 
 * Pseudocode/placeholder.
 * Retrieve the user's input.
 * Return an int (example reading).
 */
int GetUserInput();

/**
 * None -> float
 * 
 * Pseudocode/placeholder.
 * Return the Celsius temperature reading from a sensor.
 */
float GetTemperatureC();

/**
 * None -> bool
 * 
 * Pseudocode/placeholder.
 * Return true if the device's cover is present; false otherwise.
 */
bool IsCoverPresent();


/**  CONTROL FUNCTIONS  **/

/**
 * None -> None
 * 
 * Pseudocode/placeholder.
 * Turn off the device's fans.
 */
void FansOff();

/**
 * None -> None
 * 
 * Pseudocode/placeholder.
 * Turn on the device's fans.
 */
void FansOn();

/**
 * int -> None
 * val (int): placeholder value used to adjust the device's output.
 * 
 * Pseudocode/placeholder.
 * Adjust the device's output according to the input value.
 */
void SetOutput(int val);

/**
 * None -> bool
 * Return true if the initialization succeeds, false if it fails.
 * 
 * Pseudocode/placeholder.
 * Initialize the main logic board.
 */
bool MainBoardInit();

/**
 * None -> None
 * 
 * Pseudocode/placeholder.
 * Initialize the outputs.
 */
void OutputsInit();


/***  STATE HANDLER FUNCTIONS  ***/

/**
 * None -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Power-On Reset state
 */
DeviceState StatePORHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Main logic board initialization state
 */
DeviceState StateInitMainHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Halted (failed boot) state
 */
DeviceState StateHaltHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): if non-null, the following state of the device.
 * Return the device's next state.
 * 
 * Board booted, initializing outputs state
 */
DeviceState StateInitHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Normal operation state: check sensors and user inputs, adjust outputs
 */
DeviceState StateNormalHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): ignored (included for consistant function signatures)
 * Return the device's next state.
 * 
 * Adjusted state: disable fans, scale back outputs
 */
DeviceState StateSilentHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): the device's previous state, to which it will 
 *                              return after the temperature regulates.
 * Return the device's next state.
 * 
 * Error state: reduce outputs and stay in the state until the temperature regulates.
 */
DeviceState StateOverTempHandler(DeviceState previousState);

/**
 * DeviceState -> DeviceState
 * previousState (DeviceState): the device's previous state, to which it will 
 *                              return after the cover is replaced.
 * Return the device's next state.
 * 
 * Error state: disable outputs and stay in this state until the cover is replaced.
 */
DeviceState StateNoCoverHandler(DeviceState previousState);