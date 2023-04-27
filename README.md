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
#### Wiring (I'm lazy and not PRO to draw and correct the connections for readouts)
###### Breakout
![Breakout](https://github.com/pywugate/RotaryEncoder_Reward/blob/3a97ebebdc2417a64fa36b13af449324326d1b36/RotaryEncoder_Reward_bb2023Mar.jpg)
###### Schmatic


## Data acquisition
### IMPORTANT : the sampling rate is hugely hugely different between devices, pay attention at processing.
### Saleae Logic Pro8 (software Logic2)
It's is easy to use, but caution that sampling rate in digital and analog.
The sampling rate I used
 - 6.25 MS/s for Digital
 - 1250 S/s  for Analogue (Remember binning the rate for analogue to match rate of encoder)

### Femtonics SMART



## Reference
TBA/TBD
