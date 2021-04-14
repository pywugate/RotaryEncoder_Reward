/* 
 *  
 *  YUMO E6B2-CWZ3E OPTICAL ENCODER 
 *  Brown : 5V \ Blue : GND \ Exposed : GND \ Black : D2(OUTPUT A) \ White : D3(OUTPUT B) \ Orange : D4(OUTPUT Z)
 *  Maximum permissible speed : 6000 r/min = 1 r/ 10 ms (so 100Hz)
 *  
 *  Libraries used in this program
 *  1. github.com/GreyGnome/EnableInterrupt
 *  2. github.com/NicksonYap/digitalWriteFast
 *  3. github.com/JChristensen/Timer
 *  4. github.com/adafruit/Adafruit_MCP4725
 *  5. arduino.cc/en/reference/wire
 *   
 *  v = dL / t = 2 * r * PI / (N * t)
 *  r: radius \ N: resolution of encoder \ t: sampling period
 *  
 *  ω = (2 * π * n) / (N *t)
 *  ω: rad/s \ n: number of pulses \ t: sampling period \ N: resolution of encoder
 *  
 */
// Libraries
#include <EnableInterrupt.h>          //insufficient number of standard Interrupt pins on the Arduino Uno
#include <digitalWriteFast.h>
#include <Wire.h>
#include "Timer.h"
#include <Adafruit_MCP4725.h>
#include <Adafruit_BusIO_Register.h>
Adafruit_MCP4725 dac;

// Input Pin site
#define encoderA 2
#define encoderB 3
#define lever 5

// Output Pin site
#define out_relay 8
#define out_reward 9
#define out_lever 11
#define out_dir 12
#define out_LED 13

//variables for encoder
int encoderlimit = 10;                                    // Maximum permissible speed : 6000 r/min = 1 r/ 10 ms
int N = 1024;                                             // This variable is unique to the type of encoder
//double circum = 2*PI*27;                                   // avg radius = 27.125 mm
double dL =  2*PI*27/N;                                         // dL = cirum / N (mm)

//variables for motion
long Now;
long Prev;
long Dist;
long counter;                                             // for reward rule counting
bool Dir;                                                 // 1 = CW, 0 = CCW
int outlier = 200;                                        // set max difference of Dist = 200

//variables for reward
int rule = N*3;                                           // reward rules:  3 for training; 10 for exps
long count_reward;
bool lever_click;
int reward;

//variables for MCP4725 12bit DAC
int scale = 200;                                            // a scale make number beautiful
//double unit_volt = 5000/4095;                              // 5000mV / 4095
float outputV = 0;

//variables for Timer
const unsigned long PERIOD = 250;                         // reward duration, depending on the water valve usage
Timer t;                                                   // initiate the timer object

void setup() {
  Serial.begin(9600);                                      // change higher number if need higher sampling rate
  dac.begin(0x62);
  
  pinModeFast(encoderA, INPUT);                            //  pinMode(encoderA, INPUT);
  pinModeFast(encoderB, INPUT);                            //  pinMode(encoderB, INPUT);
  pinMode(lever, INPUT_PULLUP);

  pinMode(out_relay, OUTPUT);
  pinMode(out_reward, OUTPUT);
  pinMode(out_lever, OUTPUT);
  pinMode(out_dir,OUTPUT);
  pinMode(out_LED,OUTPUT);
//  digitalWrite(out_relay,LOW);
//  digitalWrite(out_reward,LOW);
  
  enableInterrupt(encoderA, motions, RISING);
  enableInterrupt(lever, click_reward, CHANGE);

  Prev = 0;
}

void location(){
  if((bool) digitalReadFast(encoderB) ==  HIGH){           //digitalReadFast() is faster than digitalRead()
    counter ++;
    count_reward ++;
  }else{
    counter --;
    count_reward --;
  }
  
}

void motions(){  
  location();
  if (Dist < 0){
    Dir = 1;                                               // if rotate CW
    digitalWrite(out_dir,HIGH);
    digitalWrite(out_LED,HIGH);
  }else{
    Dir = 0;
    digitalWrite(out_dir,LOW);
    digitalWrite(out_LED,LOW);
  }
}

void click_reward(){
  if ((bool) digitalRead(lever) == HIGH)
  {
    digitalWrite(out_lever,HIGH);
    lever_click = 1;
    if ( count_reward > rule )
    {
      t.pulseImmediate(out_relay,PERIOD,HIGH);         // use Timer to turn on reward for specific period
                                                       // Important: use a resistor
      t.pulseImmediate(out_reward,PERIOD,HIGH);
      count_reward = 0;
      //      reward = 1;
//      delay(500);
//      reward =0;
     }
  }else{
      digitalWrite(out_lever,LOW);
      lever_click = 0;
  }
}

void loop() {
  
/*  IMPORTANT:
 *  we need a delay time to calculate the difference
 *  the calculation of movement cannot be triggered by interrupt funct, bcz the device will stop recording
 */
  Now = counter;
//  Serial.print(Now);                        
//  Serial.print('\t');
  Dist = Now - Prev; 
  if ( abs(Dist) >=  outlier){
    Dist = 0;
  }
  outputV = abs(Dist)*dL*scale;
  dac.setVoltage(outputV, false);
  
  Prev = counter;
//  Serial.print(Prev);                        
//  Serial.print('\t');
  delay(encoderlimit);                                          // delay x ms to calculate distance

    Serial.print(Dir);  
    Serial.print('\t');
    Serial.print(Dist);
    Serial.print('\t');
    Serial.print(lever_click);
    Serial.print('\t');
    Serial.print(reward);
    Serial.print('\t'); 
//    Serial.print(dL);
//    Serial.print('\t');
    Serial.print(counter);
    Serial.print('\t');
    Serial.print(count_reward);
    Serial.print('\t');
//    Serial.print(rule);
//    Serial.print('\t');
//    Serial.print(unit);                        
//    Serial.print('\t');
//    Serial.print(outputV);
//    Serial.print('\t');
//    Serial.print(unit_volt);                        
//    Serial.print('\t');
    Serial.println(' ');
    t.update();
}
