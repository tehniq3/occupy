/*
 * original program by Nicu FLORICA (niq_ro)
 * 0 - intial (momentary switch at D2__/__GND)
 * 0a - added on time in serial hh:mm:ss
 * 1 - changed to record just if a button was pushed (simulated sensor) for long time
 */

#define senzor 2
#define LED0 13

byte staresenzor1 = 1; // stare actuala senzor
byte staresenzor0 = 1; // stare anterioara senzor
byte actionare = 0;    // numere pare (parcare libera)

unsigned long timp1;
unsigned long timp2;
unsigned long timp0;
unsigned long pauza = 3000; // 


void setup() {
  Serial.begin(9600);
  Serial.println(" ");
  Serial.println("Occupy Area monitoring system with momentary button");
  Serial.println("Program by Nicu FLORICA (niq_ro)");
  Serial.println("ver. 1 - 23.01.2025, Craiova");
  Serial.println("---------------------");
  
  pinMode (LED0, OUTPUT);
  digitalWrite(LED0, 0);
  pinMode (senzor, INPUT);
  digitalWrite(senzor, HIGH);
}


void loop() {
staresenzor1 = digitalRead(senzor);  // se citeste starea actuala a senzorului
 if ((actionare == 0) and (staresenzor1 == LOW)) // este activat
   { 
      Serial.println("Switch was activated !");
      timp2 = millis()/1000;  // store the time for first push record
      actionare = 1;
      delay(pauza);  // wait for debounce the false timmings
   }
staresenzor1 = digitalRead(senzor);  // se citeste starea actuala a senzorului
 if (actionare == 1)
   { 
   if (staresenzor1 == LOW) // daca inca este activat
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
staresenzor1 = digitalRead(senzor);  // se citeste starea actuala a senzorului
 if (actionare == 2)
   { 
   if (staresenzor1 == HIGH) // daca nu mai e activat
   { 
      Serial.println("--------------------");
      Serial.println("Switch was released !");
      //actionare = 0; // good news
      timp1 = millis()/1000;  // store the time for first push record
      timp0 = timp1 - timp2;
      Serial.print("Switch was activated for: ");
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
delay(100);
}  // sfarsit de program principal
