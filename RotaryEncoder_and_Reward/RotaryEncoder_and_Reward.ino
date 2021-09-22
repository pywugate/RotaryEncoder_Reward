/* 2021 Sep. 21
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
#define lever    5
#define lick     6

// Output Pin site
#define out_relay   8
#define out_reward  9
#define out_lick    10
#define out_lever   11
#define out_dir     12                                  // output by DAC is using SDA pin, so pin12 is not in use
//#define out_LED     13

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
//double unit_volt = 5000/4095;                           // 5000mV / 4095

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
//  pinMode(out_LED,OUTPUT);
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
//    digitalWrite(out_LED,LOW);
  }else if (Dist > 0){
    Dir = 1;
    digitalWrite(out_dir,HIGH);
//    digitalWrite(out_LED,HIGH);
  }else{
    Dir = 0;
  }
}

 void lever_click(){
  if ((bool) digitalRead(lever) == HIGH) {
    digitalWrite(out_lever,HIGH);
    lever_state = 1;
    give_reward();
  }else{
    digitalWrite(out_lever,LOW);
    lever_state = 0;
  }
}

void give_reward(){
  if ( count_reward > rule ){
    t.pulseImmediate(out_relay,PERIOD,HIGH);             // use Timer to turn on reward for specific period
                                                         // ?? Important: use a resistor 
    t.pulseImmediate(out_reward,PERIOD,HIGH);
    count_reward = 0;
//    reward = 1;
//    delay(500);
//    reward =0;
   }
}

void lick_chk(){
  if ((bool) digitalRead(lick) == HIGH){
    digitalWrite(out_lick,HIGH);
//    lick_state = 1;                                     // only for output in Serial Plotter
  }else{
    digitalWrite(out_lick,LOW);
//    lick_state = 0;  
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

  delay(encoderlimit*1);                                          // delay x ms to calculate distance
  
//    Serial.print(counter);
//    Serial.print('\t');
//
//    Serial.print(Dir);  
//    Serial.print('\t');
//
//    Serial.print(Dist);                                // it becomes noisy when wheel put on, why?
//    Serial.print('\t');
//
//    Serial.print(count_reward);
//    Serial.print('\t');
//
//    Serial.print(outputV);
//    Serial.print('\t');
//
//    Serial.print(dL);
//    Serial.print('\t');
//
//    Serial.print(rule);
//    Serial.print('\t');
//
//    Serial.print(lever_state);
//    Serial.print('\t');
//
//    Serial.print(lick_state);
//    Serial.print('\t');

//    Serial.print(reward);
//    Serial.print('\t'); 
//    Serial.print(unit_volt);                        
//    Serial.print('\t');


    Serial.println(' ');
    t.update();
}
