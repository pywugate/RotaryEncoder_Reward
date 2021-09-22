/* 2021 Sep. 21
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
#define encoderA    2
#define encoderB    3
#define lever       5
#define lick        6

// Output Pin site
#define out_relay   8
#define out_reward  9
#define out_lick    10
#define out_lever   11
#define out_dir     12                                  // output by DAC is using SDA pin, so pin12 is not in use

//fixed figures for encoder, these figures are unique to the type of encoder
int encoderlimit = 10;                                    // Maximum permissible speed : 6000 r/min = 1 r/ 10 ms
const int N = 1024;                                             
const int r = 110;                                        // avg radius = 110 mm => circum = 2*PI*r;                               
double dL =  2*PI*r / N;                                  // dL = cirum / N (mm)

//variables for motion
long Now;
long Prev;
long Dist;
long counter;                                             
int Dir;                                                  // 0 = CW, 1 = CCW
int outlier = 200;                                        // set max difference of Dist = 200

//variables for MCP4725 12bit DAC
int scale = 100;                                          // a scale make analogue signal beautiful
float outputV = 0;

//variables for reward and licking
int rule = N*3;                                           // reward rules:  3 for training; 10 for exps
int reward;
long count_reward;                                        // for reward  counting
const unsigned long PERIOD = 250;                         // reward duration, depending on the water valve usage

// output only in Serial Plotter
bool lever_state;
bool lick_state;

Timer t;                                                  // initiate the timer object

void setup() {
  Serial.begin(9600);                                     // change higher number if need higher sampling rate
  dac.begin(0x62);
  
  pinModeFast(encoderA, INPUT);                           // faster than pinMode(encoderA, INPUT);
  pinModeFast(encoderB, INPUT);                           // faster than pinMode(encoderB, INPUT);
  pinMode(lever, INPUT_PULLUP);
  pinMode(lick, INPUT_PULLUP);

  pinMode(out_relay, OUTPUT);
  pinMode(out_reward, OUTPUT);
  pinMode(out_lick, OUTPUT);
  pinMode(out_lever, OUTPUT);
  pinMode(out_dir,OUTPUT);
  digitalWrite(out_relay,LOW);
  digitalWrite(out_reward,LOW);
  
  enableInterrupt(encoderA, motions, RISING);
  enableInterrupt(lever, lever_click, CHANGE);
  enableInterrupt(lick, lick_chk, CHANGE);
  Prev = 0;
}

void location(){
  if((bool) digitalReadFast(encoderB) == HIGH){           //digitalReadFast() is faster than digitalRead()
    counter ++;
    count_reward ++;
  }else if ((bool) digitalReadFast(encoderB) == LOW){     // HONTKO encoders could not work without this line
    counter --;
    count_reward --;
  }
}

void motions(){  
  location();
  if (Dist < 0){
    Dir = 0;                                               // if rotate CCW
    digitalWrite(out_dir,LOW);
  }else if (Dist > 0){
    Dir = 1;
    digitalWrite(out_dir,HIGH);
  }else{
    Dir = 0;
  }
}

 void lever_click(){
  if ((bool) digitalRead(lever) == HIGH) {
    digitalWrite(out_lever,HIGH);
    give_reward();
  }else{
    digitalWrite(out_lever,LOW);
  }
}

void give_reward(){
  if ( count_reward > rule ){
    t.pulseImmediate(out_relay,PERIOD,HIGH);             // use Timer to turn on reward for specific period
    t.pulseImmediate(out_reward,PERIOD,HIGH);
    count_reward = 0;
   }
}

void lick_chk(){
  if ((bool) digitalRead(lick) == HIGH){
    digitalWrite(out_lick,HIGH);
  }else{
    digitalWrite(out_lick,LOW);
  }
}

void loop() {
  
/*  IMPORTANT:
 *  a delay time needed to calculate the difference
 *  the calculation of movement cannot be triggered by interrupt funct, bcz the device will stop recording
 */
 
  Now = counter;
  Dist = Now - Prev; 
  if ( abs(Dist) >=  outlier){
    Dist = 0;
  }  
  outputV = abs(Dist)*dL*scale;
  dac.setVoltage(outputV, false);
  
  Prev = counter;
  delay(encoderlimit*1);                                          // delay x ms to calculate distance
  Serial.println(' ');
  t.update();
}
