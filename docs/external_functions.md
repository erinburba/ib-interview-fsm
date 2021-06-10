# External Functions
Documentation of the pseudocode functions used in my program. Noting their names, signatures, occurrences, and the functions they represent.


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
<b>Signature:</b> none -> none
<b>Occurrences:</b> in StateInitMainHandler
Represents initializing the main logic board. Assume that an error in initialization causes an interrupt or event.


### OutputsInit
<b>Signature:</b> none -> none

<b>Occurrences:</b> in StateInitHandler

Represents initializing the device's sensors, inputs, and outputs after the main logic board has booted.
