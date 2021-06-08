

/******  CONSTANTS  ******/

// Threshold temperature to define overheating.
#define MAX_TEMP 100

// Unit-less value of what the output should be at standard operation.
#define NORMAL_OUTPUT 100

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

/******  FUNCTION DECLARATIONS  ******/

/***  PSEUDOCODE PLACEHOLDERS  ***/

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
 * Return the temperature reading from a sensor.
 */
float GetTemp();

/**
 * None -> bool
 * 
 * Pseudocode/placeholder.
 * Return true if the device's cover is present; false otherwise.
 */
bool IsCoverPresent();

/**
 * None -> bool
 * 
 * Pseudocode/placeholder.
 * Return true if the device is set to silent mode; false otherwise.
 */
bool IsSilentMode();


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
 * None -> None
 * 
 * Pseudocode/placeholder.
 * Disable the device's outputs.
 */
void DisableOutput();

/**
 * None -> None
 * 
 * Pseudocode/placeholder.
 * Initialize the main logic board.
 */
void MainBoardInit();

/**
 * None -> None
 * 
 * Pseudocode/placeholder.
 * Initialize the outputs.
 */
void OutputsInit();


/***  STATE HANDLER FUNCTIONS  ***/

/**
 * None -> None
 * 
 * Power-On Reset state
 */
void StatePORHandler();

/**
 * None -> None
 * 
 * Main logic board initialization state
 */
void StateInitMainHandler();

/**
 * None -> None
 * 
 * Halted (failed boot) state
 */
void StateHaltHandler();

/**
 * DeviceState -> None
 * overrideState (DeviceState): if non-null, the following state of the device.
 * 
 * Board booted, initializing outputs state
 */
void StateInitHandler(DeviceState overrideState); //TODO: remove overrideState if it doesn't fit the clarified specs.

/**
 * None -> None
 * 
 * Normal operation state: check sensors and user inputs, adjust outputs
 */
void StateNormalHandler();

/**
 * None -> None
 * 
 * Adjusted state: disable fans, scale back outputs
 */
void StateSilentHandler();

/**
 * DeviceState -> None
 * previousState (DeviceState): the device's previous state, to which it will 
 *                              return after the temperature regulates.
 * 
 * Error state: reduce outputs and stay in the state until the temperature regulates.
 */
void StateOverTempHandler(DeviceState previousState);

/**
 * DeviceState -> None
 * previousState (DeviceState): the device's previous state, to which it will 
 *                              return after the cover is replaced.
 * 
 * Error state: disable outputs and stay in this state until the cover is replaced.
 */
void StateNoCoverHandler(DeviceState previousState);