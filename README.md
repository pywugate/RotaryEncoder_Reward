# RotaryEncoder_Reward

RotaryEncoder_Reward was designed for mouse behaviour experiment for head-fixed mouse.

Please contact me via github if any question.

##### IMPORTANT: The parametres may vary due to different components.

## Components used:
1. Arduino UNO R3
2. Rotary Encoder
 -  YUMO E6B2-CWZ3E OPTICAL ENCODER or equivalent
    * Brown: 5V
    * Blue: GND
    * Black: D2(OUTPUT A)
    * White: D3(OUTPUT B)
    * Orange: D4(OUTPUT Z)
    * Exposed: Shield
    * Maximum permissible speed : 6000 r/min = 1 r/ 10 ms (so 100Hz)
3. Microswitch
4. Relay Module (JQC-3FF-S-Z)
6. Electric valve
7. DAC (MCP4725)
8. Electric components (wires, resistors, breadboard, etc)
9. Thin metal tube

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
### IMPORTANT : the sampling rate is hugely hugely different between devices, pay attention at processing.
### Saleae Logic Pro8 (software Logic 2.3)
It's is easy to use, but caution that sampling rate in digital and analog.
The sampling rate I used
 - 6.25 MS/s for Digital
 - 250 S/s  for Analogue (Remember binning the rate for analogue to match rate of encoder)

### Femtonics SMART
Our goal is combining two-photon imaging and behavioural readouts, so we send data into 2P system.

Because this is a high-end commercial system, it's very easy and friendly input setting.


## Reference
TBA/TBD
