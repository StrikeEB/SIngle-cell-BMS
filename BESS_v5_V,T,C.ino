//below note is a must-to-be-included if the current sensor library and code can be used
// this instruciton is given in line 72

/*
 * 
 * Arduino Sketch for Allegro ACS770 Current Sensor 
 * this sensor is offered at 50A, 100A, 150A and 200A range
  both for AC and DC. 
  
 * It operates with 5V
 * Please watch video instruction and explanation for this code
  Watch video instruction for ACS770 50A to 200A Sensors https://youtu.be/sB6EULTix2k
  Watch video instruction for ACS712 5A to 30A Sensors :https://youtu.be/3C33DpcSwIw

 * 
 * Written by Ahmad Shamshiri on Saturday April 04, 2020 at 21:19 in Ajax, Ontario, Canada
 * for Robojax.com
 * View the video :
 * This code has been downloaded from Robojax.com
 * 
model select
      //ACS712
      0-"ACS712ELCTR-05B",
      1-"ACS712ELCTR-20A",
      2-"ACS712ELCTR-30A",
      
      //ACS758
      3,// for ACS758LCB-050B
      4,// for ACS758LCB-050U
      5,// for ACS758LCB-100B
      6,// for ACS758LCB-100U
      7,// for ACS758KCB-150B
      8,// for ACS758KCB-150U
      9,// for ACS758ECB-200B
      10,// for ACS758ECB-200U 
      
      ///ACS770
      11,// for ACS770x-050B ///      
      12,// for ACS770x-050U  
      13,// for ACS770x-100B
      14,// for ACS770x-100U
      15,// for ACS770x-150B
      16,// for ACS770x-150U
      17,// for ACS770x-200B  
      18,// for ACS770x-200U  
      

      19 for "ACS732KLATR-20AB",
      20 for "ACS732KLATR-40AB",
      21 for "ACS732KLATR-65AB", 
      22 for "ACS732KLATR-65AU",   
      23 for "ACS732KLATR-75AB", 

      24 for "ACS733KLATR-20AB",
      25 for "ACS733KLATR-40AB",
      26 for "ACS733KLATR-40AU", 
      27 for "ACS733KLATR-65AU",

 * this code has been downloaded from http://robojax.com/learn/arduino/
 
 * Get this code and other Arduino codes from Robojax.com
Learn Arduino step by step in structured course with all material, wiring diagram and library
all in once place. Purchase My course on Udemy.com http://robojax.com/L/?id=62

****************************
Get early access to my videos via Patreon and have  your name mentioned at end of very 
videos I publish on YouTube here: http://robojax.com/L/?id=63 (watch until end of this video to list of my Patrons)
****************************

or make donation using PayPal http://robojax.com/L/?id=64

 *  * This code is "AS IS" without warranty or liability. Free to be used as long as you keep this note intact.* 
 * This code has been download from Robojax.com
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.

 */


const int Enable12 = 5;  // PWM pin to L293D's EN12 (pin 1) 
const int Driver1A = 4;  // To L293D's 1A (pin 2)
const int Driver2A = 3;  // To L293D's 2A (pin 7)

const int VIN = A3; // define the Arduino pin as voltage input (V in)
const float VCC   = 5; // current voltage supply
const int MODEL = 2; // model selected from Robojax.com list; http://robojax.com/learn/arduino/?vid=robojax_Alegro_ACS712_curren_sensor
#include <Robojax_AllegroACS_Current_Sensor.h> // current sensor library

#define ANALOG_PIN A0
#define ANALOG_PIN A1
float adc_volt = 0.0;
float adc_volt2 = 0.0;
float in_volt = 0.0;
float R1 = 30000.0;
float R2 = 75000.0;
float ref_volt = 3.7;
float temp;

int adc_value = 0;
int tempPin = 0;

Robojax_AllegroACS_Current_Sensor robojax(MODEL,VIN);

void setup()
{
       //---set pin direction
    pinMode(Enable12,OUTPUT);
    pinMode(Driver1A,OUTPUT);
    pinMode(Driver2A,OUTPUT);
 Serial.begin(9600);
 Serial.println("DC Voltage");

}

void motorCTRL(byte speed, bool D1A, bool D2A){
    /*  motorCTRL controls the DC motor
     *    speed: any value between 0-255, used as PWM
     *             0 - off
     *           255 - maximum
     *      D1A: Input 1 or 1A, boolean value of HIGH or LOW          
     *      D2A: Input 2 or 2A, boolean value of HIGH or LOW
     */
	analogWrite(Enable12,speed);  // PWM
	digitalWrite(Driver1A,D1A);   // Boolean
	digitalWrite(Driver2A,D2A);   // Boolean 
}

void loop()
{

  do_voltage();
  do_temp();
  do_current();
  do_spin();
  do_current();
}

void do_voltage()
{
adc_value = analogRead(ANALOG_PIN);
adc_volt  = (adc_value * ref_volt) / 1024.0;
in_volt = adc_volt / (R2/(R1+R2));
//in_volt = in_volt/1000000;
Serial.print("Voltage = ");
Serial.println(in_volt, 2);

delay(500);
}

void do_temp() 
{
   temp = analogRead(tempPin);
   // read analog volt from sensor and save to variable temp
   temp = temp * 0.48828125;
   // convert the analog volt to its temperature equivalent
   Serial.print("Temperature = ");
   Serial.print(temp); // display temperature value
   Serial.print("*C");
   Serial.println();
   delay(500); // update sensor reading each one second
}

void do_current() 
{
    Serial.print("Current = ");
    Serial.print(robojax.getCurrent(),3); // print the current with 3 decimal places
    Serial.print("A Avg:");
    Serial.print(robojax.getCurrentAverage(300),3);//   
    Serial.println("A");
   //robojax.debug();
   
  delay(500);
}

void do_spin()
{
motorCTRL(255,HIGH,LOW);      // one way
	delay(3000);
	motorCTRL(0,HIGH,LOW);        // slow stop
	delay(2000);
	motorCTRL(255,LOW,HIGH);      // reverse
	delay(3000);
	motorCTRL(255,LOW,LOW);       // fast stop
	delay(2000);
}







