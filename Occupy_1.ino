/*
 * original program by Nicu FLORICA (niq_ro)
 * 0 - intial (momentary switch at D2__/__GND)
 * 0a - added on time in serial hh:mm:ss
 * 1 - changed to record just if a button was pushed (simulated sensor) for long time
 * 2 - changed momentary switch with Sharp IR sensor as at https://www.makerguides.com/sharp-gp2y0a710k0f-ir-distance-sensor-arduino-tutorial/
       (if measured distance is lower than the threshold means activated sensor)
 */

#include <SharpIR.h>  // https://github.com/guillaume-rico/SharpIR
/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F  --> 430
*/

#define model 100500 // GP2Y0A710K0F
#define senzor A0
#define LED0 13

byte staresenzor1 = 1; // stare actuala senzor
byte staresenzor0 = 1; // stare anterioara senzor
byte actionare = 0;    // numere pare (parcare libera)

unsigned long timp1;
unsigned long timp2;
unsigned long timp0;
unsigned long pauza = 3000; // 
int distanta = 0;   // in cm
int distmin = 100;  // in cm

SharpIR SharpIR(senzor, model);

void setup() {
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("ON/OFF control with Sharp IR sensor");
  Serial.println("Program by Nicu FLORICA (niq_ro)");
  Serial.println("ver. 2 - 24.01.2025, Craiova");
  Serial.println("---------------------");
  
  pinMode (LED0, OUTPUT);
  digitalWrite(LED0, 0);
}


void loop() {
  distanta = SharpIR.distance();  // masoara distanta
  if ((actionare == 0) and (distanta < distmin)) // este activat
   { 
      Serial.println("Sesor was activated !");
      timp2 = millis()/1000;  // store the time for first push record
      actionare = 1;
      delay(pauza);  // wait for debounce the false timmings
   }
  distanta = SharpIR.distance();  // measurement of the distance
  if (actionare == 1)
   { 
  if (distanta < distmin)    // if is still activated
   { 
      Serial.println("Switch is still activated !");
      actionare = 2; // good news
      digitalWrite(LED0, 1);
   }
   else   
   { 
      Serial.println("False trigger !");
      timp1 = millis()/1000;  // store the time for first push record
      actionare = 0;   // reset the status as not closed switch
   } 
   }
  distanta = SharpIR.distance();  // masoara distanta
  if (actionare == 2)
   { 
   if (distanta > distmin)    // if is not activated
   { 
      Serial.println("--------------------");
      Serial.println("Sensor was released !");
      //actionare = 0; // good news
      timp1 = millis()/1000;  // store the time for first push record
      timp0 = timp1 - timp2;
      Serial.print("Sensor was activated for: ");
      Serial.print((timp0/3600)/10);
      Serial.print((timp0/3600)%10);
      Serial.print("h:");
      Serial.print(((timp0%3600)/60)/10);
      Serial.print(((timp0%3600)/60)%10);
      Serial.print("m:");
      Serial.print(((timp0%3600)%60)/10);
      Serial.print(((timp0%3600)%60)%10);
      Serial.println("s");
      Serial.println("--------------------");
      digitalWrite(LED0, 0);
      actionare = 0;
   }
   }
delay(500);
}  // sfarsit de program principal
