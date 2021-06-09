# State Transitions
A detailed description of state transitions. Includes descriptions of the states, and details the conditions that cause transitions between the states.


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


## Transitions

| State           | Achieved By                                                                                   | Possible Prior States                     | Exited By                                                       | Possible Next States                                                                                            |
|-----------------|-----------------------------------------------------------------------------------------------|-------------------------------------------|-----------------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------|
| STATE_POR       | Powering on the device                                                                        | None                                      | Beginning board initialization                                  | STATE_INIT_MAIN                                                                                                 |
| STATE_INIT_MAIN | Attempting to initialize the main logic board                                                 | STATE_POR                                 | Succeeding or failing to boot                                   | STATE_HALT, STATE_INIT                                                                                          |
| STATE_HALT      | Failing to boot                                                                               | STATE_INIT_MAIN                           | Retrying board initialization                                   | STATE_INIT_MAIN                                                                                                 |
| STATE_INIT      | Successfully booting the board, all outputs disabled                                          | STATE_INIT_MAIN, STATE_NO_COVER           | Entering a steady state based on errors or operational settings | Any steady state                                                                                                |
| STATE_NORMAL    | No operational errors, restrictions, or limitations                                           | STATE_INIT, STATE_SILENT, STATE_OVER_TEMP | Cover removal, overheating error, switch to silent mode         | Any other steady state                                                                                          |
| STATE_SILENT    | Changing the operational setting, or returning to the steady state after an error is resolved | STATE_INIT, STATE_NORMAL, STATE_OVER_TEMP | Cover removal, overheating error, switch to normal mode         | Any other steady state                                                                                          |
| STATE_OVER_TEMP | Overheating while steady or while initializing the outputs                                    | STATE_INIT, STATE_NORMAL, STATE_SILENT    | Cover removal, normal temperature detected                      | STATE_NO_COVER or the preceding steady state                                                                    |
| STATE_NO_COVER  | Removal/absence of the device cover while steady or while initializing the outputs            | STATE_INIT, any other steady state        | Replacing the cover                                             | STATE_INIT, followed by the state from before the cover was removed |
