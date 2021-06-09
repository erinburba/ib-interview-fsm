# Code Theory of Operation

A description of design choices, benefits and tradeoffs, and more (?)


## Assumptions
* When checking the conditions for the error states, the cover is checked before the temperature. This means that if in reality the device is both overheating and missing the cover upon initialization, the device will do into STATE_NO_COVER. <b>Reasoning:</b> no outputs or fans should run when the cover is removed, regardless of temperature.