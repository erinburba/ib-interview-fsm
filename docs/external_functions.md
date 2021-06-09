# External Functions
Documentation of the pseudocode functions used in my program. Noting their names, signatures, occurrences, and the functions they represent.


## Interrupt Handling
### HandleSilentModeButtonPress
<b>Signature:</b> int irq -> None

<b>Occurrences:</b> none; placeholder for implementation-specific logic.

Placeholder to demostrate how to achieve toggling between normal and silent modes through the use of interrupts. Pressing the button would trigger a hardware interrupt, and a handler would be written to recognize that action and toggle the variable that controls silent/normal operation.


## Input Functions
### GetUserInput
<b>Signature:</b> none -> int

<b>Occurrences:</b> in StateNormalHandler

Represents reading and returning an input from the user. In this case, imagined as a value 0-100 that is used to control the device's output.

### GetTemperatureC
<b>Signature:</b> none -> float

<b>Occurrences:</b> in ErrorDetection, StateOverTempHandler

Represents reading from a temperature sensor and returning the current temperature in Celsius.

### IsCoverPresent
<b>Signature:</b> none -> bool

<b>Occurrences:</b> in ErrorDetection, StateOverTempHandler, StateNoCoverHandler

Represents reading from a sensor that checks for presence of the device's cover. Returns true if the cover is present, and false otherwise.

## Control Functions
### FansOff
<b>Signature:</b> none -> none

<b>Occurrences:</b> in StateSilentHandler, StateNoCoverHandler

Represents disabling the device's fans.

### FansOn
<b>Signature:</b> none -> none

<b>Occurrences:</b> in StateNormalHandler, StateOverTempHandler

Represents enabling and powering the device's fans.

### SetOutput
<b>Signature:</b> int val -> none

<b>Occurrences:</b> in StateNormalHandler, StateSilentHandler, StateNoCoverHandler, StateOverTempHandler

Represents adjusting the device's output. The parameter val is imagined as a value 0-100, corresponding to the input read in GetUserInput.

### MainBoardInit
<b>Signature:</b> none -> bool

<b>Occurrences:</b> in StateInitMainHandler

Represents initializing the main logic board. Returns true if initializing succeeded, or false if it failed to boot.


### OutputsInit
<b>Signature:</b> none -> none

<b>Occurrences:</b> in StateInitHandler

Represents initializing the device's sensors, inputs, and outputs after the main logic board has booted.
