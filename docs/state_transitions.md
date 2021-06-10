# State Transitions
A detailed description of state transitions. Includes descriptions of the states, list of events that can cause state transitions, and a condensed table detailing state transitions defined as part of this FSM.

## Initializing States
Initializing states describe the device's conditions as it powers on, attempts to initialize the main logic board, and retries initialization as needed until the device is ready for standard operation.
* STATE_POR - Power-On Reset; first powers on and comes online, before any booting.
* STATE_INIT_MAIN - Initialize Main Board; attempting to initialize the main logic board
* STATE_HALT - Halt Mode; failed to initialize the board
* STATE_INIT - Device Initialize; board initialized successfully, initializing the device's outputs

## Steady States
Steady states describe the conditions the device can be in after a successful boot and initialization. 
* STATE_NORMAL - Normal Operation; check sensors and user inputs, adjust outputs
* STATE_SILENT - Silent Operation; disable fans, scale back outputs
* STATE_OVER_TEMP - Over Temp State; an Error state reached when the device overheats; reduce outputs
* STATE_NO_COVER - Cover Removed State; an Error state caused by absence of the cover; outputs disabled for safety

## Events That Cause Transitions
These events can cause transitions between the states; notably not every event causes a transition from every state.
* Successful power on/pre-init
* Successful board booting
* Failed board booting
* Signal to retry booting
* Successful standard output initialization
* Pushing the silent mode toggle button
* Temperature rising too high
* Temperature returning to normal after overheating
* Removing the cover
* Replacing the cover
* User input changing (doesn't really cause a transition, but requires an update in normal operation)


## Transitions

| State           | Possible Prior States                     | Exited By                                                                                                | Next State                                                          |
|-----------------|-------------------------------------------|----------------------------------------------------------------------------------------------------------|---------------------------------------------------------------------|
| STATE_POR       | None                                      | Beginning board initialization                                                                           | STATE_INIT_MAIN                                                     |
| STATE_INIT_MAIN | STATE_POR                                 | Successfully booting <br>Failing to boot                                                                 | STATE_INIT<br>STATE_HALT                                            |
| STATE_HALT      | STATE_INIT_MAIN                           | Retrying board initialization                                                                            | STATE_INIT_MAIN                                                     |
| STATE_INIT      | STATE_INIT_MAIN, STATE_NO_COVER           | Cover removal/missing<br>Overheating error<br>Press silent mode button<br>Standard output initialization | STATE_NO_COVER<br>STATE_OVER_TEMP<br>STATE_SILENT<br>STATE_NORMAL   |
| STATE_NORMAL    | STATE_INIT, STATE_SILENT, STATE_OVER_TEMP | Cover removal<br>Overheating error<br>Press silent mode button                                           | STATE_NO_COVER<br>STATE_OVER_TEMP<br>STATE_SILENT                       |
| STATE_SILENT    | STATE_INIT, STATE_NORMAL, STATE_OVER_TEMP | Cover removal<br>Overheating error<br>Press silent mode button                                           | STATE_NO_COVER<br>STATE_OVER_TEMP<br>STATE_NORMAL                   |
| STATE_OVER_TEMP | STATE_INIT, STATE_NORMAL, STATE_SILENT    | Cover removal<br>Normal temperature detected                                                             | STATE_NO_COVER<br>The preceding steady state                        |
| STATE_NO_COVER  | STATE_INIT, any other steady state        | Replacing the cover                                                                                      | STATE_INIT, followed by the preceding steady state                  |