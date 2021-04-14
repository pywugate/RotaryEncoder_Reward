# RotaryEncoder_Reward

RotaryEncoder_Reward was designed for mouse behaviour experiment for head-fixed mouse.

Please contact me via github if any question.

##### IMPORTANT: The parametres may vary due to different components.

## Components used:
1. Arduino UNO R3
2. Rotary Encoder
3. Microswitch
4. Relay Module (JQC-3FF-S-Z)
6. Electric valve
7. DAC (MCP4725)
8. Electric components (wires, resistors, breadboard, etc)

## Libraries used:
Please dig in the detail in my Arduino code and the following libs.
1. github.com/GreyGnome/EnableInterrupt
2. github.com/NicksonYap/digitalWriteFast
3. github.com/JChristensen/Timer
4. github.com/adafruit/Adafruit_MCP4725
5. arduino.cc/en/reference/wire

## Example photos and/or videos
#### Wiring 
###### Breakout
![Breakout](https://github.com/pywugate/RotaryEncoder_Reward/blob/b42652cc0a700b8c1a3ae59be8ff0ebda8aa9c27/RotaryEncoder_Reward_bb.jpg)
###### Schmatic
![Schmatic](https://github.com/pywugate/RotaryEncoder_Reward/blob/ff5aa5d5ec1e675ef749c29bd2ef246359cd9f5a/RotaryEncoder_Reward_schem.jpg)

## Data acquisition
##### IMPORTANT : the sampling rate is hugely hugely different between devices, pay attention at processing.
#### Saleae Logic Pro8 (software Logic 2.3)
It's is easy to use, but caution that sampling rate in digital and analog.
The sampling rate I used
 - 6.25MS/s for Digital
 - 250 S/s  for Analog
##### Remember binning the rate for analog (to match rate of encoder)

#### Femtonics SMART
Our goal is combining two-photon imaging and behavioural readouts, so we send data into 2P system. \n
Because this is a high-end commercial system, it's very easy and friendly input setting.


## Reference
TBA/TBD
