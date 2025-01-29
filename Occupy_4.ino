/*
 * original program by Nicu FLORICA (niq_ro)
 * v.0 - intial (momentary switch at D2__/__GND)
 * v.0a - added on time in serial hh:mm:ss
 * v.1 - changed to record just if a button was pushed (simulated sensor) for long time
 * v.2 - changed momentary switch with Sharp IR sensor as at https://www.makerguides.com/sharp-gp2y0a710k0f-ir-distance-sensor-arduino-tutorial/
       (if measured distance is lower than the threshold means activated sensor)
 * v.3 - added 3 leds (red - occupy, yellow - initial state for occupy, gree - free space)
 * v.4 - added TM1637 display (4 digits) to show last occupy time
 */

#include <SharpIR.h>  // https://github.com/guillaume-rico/SharpIR
#include <TM1637Display.h>  // https://github.com/avishorp/TM1637

/* Model :
  GP2Y0A02YK0F --> 20150
  GP2Y0A21YK0F --> 1080
  GP2Y0A710K0F --> 100500
  GP2YA41SK0F  --> 430
*/

#define model 100500 // GP2Y0A710K0F
#define senzor A0
#define CLK 2   // TP1637 display
#define DIO 3   // TP1637 display
#define LED0 13 // internal led on-board
#define LED1 4  // green led
#define LED2 5  // yellow led
#define LED3 6  // red led

byte actionare = 0;    // numere pare (parcare libera)

unsigned long timp1;
unsigned long timp2;
unsigned long timp0;
unsigned long pauza = 3000; // 
int distanta = 0;   // distance in cm
int distmin = 100;  // threshold distance in cm

SharpIR SharpIR(senzor, model);
TM1637Display display(CLK, DIO);
int k = 1;
int h,m,s;

void setup() {
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Occupy Area monitoring system with Sharp IR sensor");
  Serial.println("Program by Nicu FLORICA (niq_ro)");
  Serial.println("ver.3 - 24.01.2025, Craiova");
  Serial.println("---------------------");
  
  pinMode (LED0, OUTPUT);
  digitalWrite(LED0, 0);
  pinMode (LED1, OUTPUT);
  digitalWrite(LED1, 0);
  pinMode (LED2, OUTPUT);
  digitalWrite(LED2, 0);
  pinMode (LED3, OUTPUT);
  digitalWrite(LED3, 0);
  display.setBrightness(2);  // 0-7
}


void loop() {
  distanta = SharpIR.distance();  // masoara distanta
  if ((actionare == 0) and (distanta < distmin)) // este activat
   { 
      Serial.println("Sesor was activated !");
      timp2 = millis()/1000;  // store the time for first push record
      actionare = 1;
      digitalWrite(LED1, 0);  // red led
      digitalWrite(LED2, 1);  // yellow led
      digitalWrite(LED3, 0);  // green led
      delay(pauza);  // wait for debounce the false timmings
   }
   if ((actionare == 0) and (distanta >= distmin)) // este neactivat
   { 
      digitalWrite(LED1, 0);  // red led
      digitalWrite(LED2, 0);  // yellow led
      digitalWrite(LED3, 1);  // green led
   }
  distanta = SharpIR.distance();  // measurement of the distance
  if (actionare == 1)
   { 
  if (distanta < distmin)    // if is still activated
   { 
      display.clear();
      Serial.println("Switch is still activated !");
      actionare = 2; // good news
      digitalWrite(LED0, 1);
      digitalWrite(LED1, 1);  // red led
      digitalWrite(LED2, 0);  // yellow led
      digitalWrite(LED3, 0);  // green led
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
      h = timp0/3600;
      Serial.print("h:");
      Serial.print(((timp0%3600)/60)/10);
      Serial.print(((timp0%3600)/60)%10);
      m = (timp0%3600)/60;
      Serial.print("m:");
      Serial.print(((timp0%3600)%60)/10);
      Serial.print(((timp0%3600)%60)%10);
      s = (timp0%3600)%60;
      Serial.println("s");
      Serial.println("--------------------");
      digitalWrite(LED0, 0);
     // display.showNumberDec(dis, false);
     // display.showNumberDec(dis, true);
      display.showNumberDecEx(100*m+s, (0x80 >> k%2), true); 
      actionare = 0;
   }
   }
delay(500);
}  // sfarsit de program principal
