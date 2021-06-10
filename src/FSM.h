/**
 * Erin Burba
 * Impact Biosystems: Firmware Interview
 * FSM.h
 * 
 * Header file for my implementation of the Finite State Machine
 * described here: https://www.notion.so/Finite-State-Machine-3b9a2982f4344f1c997372f579a02bd7
 */


/* ################  CONSTANTS  ################ */

// Unit-less 0-100 output level that results in no output.
#define NO_OUTPUT 0

// Unit-less 0-100 output level value for reduced/silent operation.
#define REDUCED_OUTPUT 40

// Number of distinct state transitions.
# define N_STATE_TRANSITIONS 18

/* ################  TYPEDEFS  ################ */

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

// Different events that occur on the device
typedef enum {
    EVENT_POR_DONE,
    EVENT_BOARD_BOOT_RETRY,
    EVENT_BOARD_BOOT_FAIL,
    EVENT_BOARD_BOOT_SUCCESS,
    EVENT_BOOT_SUCCESS,
    EVENT_SILENT_BUTTON,
    EVENT_TEMPERATURE_HIGH,
    EVENT_TEMPERATURE_NORMAL,
    EVENT_COVER_OFF,
    EVENT_COVER_ON,
    EVENT_USER_INPUT // wouldn't cause a state change, but would require an update in normal operation
} DeviceEvent;

// Function pointer defining state transition functions.
typedef DeviceState (*EventHandler) (DeviceState);

// Struct unifying a device state with an event it should
// respond to, and the function to handle that response.
typedef struct {
    DeviceState state;
    DeviceEvent event;
    EventHandler handler;
} StateMachine;

/* ################  FUNCTION DECLARATIONS  ################ */

/************  PSEUDOCODE PLACEHOLDERS  ************/

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

/**
 * None -> DeviceEvent
 * Return the identifier of an event when one occurs.
 * 
 * Pseudocode/placeholder.
 * Wait for an event to happen on the device, and return that event's identifier.
 */
DeviceEvent GetEvent();

/**
 * None -> int
 * Return the user's input value.
 * 
 * Pseudocode/placeholder.
 * Read a value from the user's input (sensor, setting, etc). Returned value is used
 * to set the device's output in standard operation mode.
 */
int GetUserInput();

/************  STATE HANDLER FUNCTIONS  ************/

DeviceState StatePORHandler(DeviceState previousState);

DeviceState StateInitMainHandler(DeviceState previousState);

DeviceState StateHaltHandler(DeviceState previousState);

DeviceState StateInitHandler(DeviceState previousState);

DeviceState StateNormalHandler(DeviceState previousState);

DeviceState StateSilentHandler(DeviceState previousState);

DeviceState StateOverTempHandler(DeviceState previousState);

DeviceState StateNoCoverHandler(DeviceState previousState);

DeviceState StateNormalTempHandler(DeviceState previousState);